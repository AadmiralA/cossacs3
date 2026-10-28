#!/usr/bin/env python3
"""Offline smoke tests of generated Gameplay Overhaul source. No game engine needed.

On Windows, first execute:
    powershell -File tools/build_gameplay_overhaul_diagnostics.ps1 -LogMode Compact
Then:
    python -m unittest discover -s tools -p test_overhaul_offline.py -v
"""
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]
GENERATED = ROOT / "mod" / "data"
sys.path.insert(0, str(ROOT / "tools"))
import analyze_overhaul_log  # noqa: E402


class GeneratedModTests(unittest.TestCase):
    paths = (
        "scripts/dmscript.global",
        "scripts/units/unit.inc/initial.inc",
        "scripts/units/unit.inc/nothing.inc",
        "scripts/units/global.inc/progresseconomicai.inc",
        "scripts/lib/unit.script",
        "scripts/lib/miscext2.script",
        "scripts/lib/country.script",
        "scripts/lib/player.script",
    )

    def read(self, relative):
        path = GENERATED / relative
        self.assertTrue(path.is_file(), f"Missing generated file: {path}")
        data = path.read_bytes()
        self.assertGreater(len(data), 200, f"Empty or truncated file: {path}")
        return data.decode("latin-1")

    def test_all_eight_overrides_generated(self):
        for relative in self.paths:
            with self.subTest(relative=relative):
                self.read(relative)

    def test_artillery_slowed_and_both_reload_slots(self):
        initial = self.read("scripts/units/unit.inc/initial.inc")
        unit = self.read("scripts/lib/unit.script")
        self.assertIn("SetGameObjectMyTrackPointMoveStep(0.006)", initial)
        self.assertIn("objbase.weapon[0].pause := objbase.weapon[0].pause*1.5", unit)
        self.assertIn("objbase.weapon[1].pause := objbase.weapon[1].pause*1.5", unit)
        self.assertIn("OVDIAG GUN_INIT", initial)
        self.assertIn("OVDIAG RELOAD", unit)

    def test_formation_penalty_after_armor(self):
        damage = self.read("scripts/lib/miscext2.script")
        unit = self.read("scripts/lib/unit.script")
        self.assertIn("var pOverhaulNoFormation : Boolean = False;", damage)
        self.assertIn("damage := Max(1, Round(damage*0.625));", damage)
        self.assertEqual(damage.count("if pOverhaulNoFormation then"), 2,
                         "Expected protection-after-armor penalty in both paths")
        self.assertIn("OVDIAG HP_APPLY", damage)
        self.assertIn("bArtillery", unit)

    def test_gold_maintenance_disabled_and_once_per_crisis_revolt(self):
        units = self.read("scripts/lib/unit.script")
        player = self.read("scripts/lib/player.script")
        tick = self.read("scripts/units/unit.inc/nothing.inc")
        self.assertIn("objprop.consume[gc_resource_type_gold] := 0;", units)
        self.assertIn("ovrWasGoldCrisis", player)
        self.assertIn("if RandomExt<0.25 then", player)
        self.assertIn("OVDIAG GOLD_SQUAD_REVOLT", player)
        self.assertIn("OVDIAG GOLD_CRISIS", player)
        self.assertIn("OVDIAG GOLD_RECOVERED", player)
        self.assertIn("gPlayer[plInd].brebellion := ovrNowGoldCrisis;", player)
        self.assertIn("_misc_ChangePlayer(ovrUnitHnd, ovrMercHnd", player)
        self.assertIn("if (not _net_IsOffline) and (gPlayer[plInd].brebellion)", tick)

    def test_second_stone_cart_upgrade_reduced(self):
        country = self.read("scripts/lib/country.script")
        self.assertIn(
            "upgplace+'.23', 1, gc_upg_tooltiptype_none, gc_upg_type_effectstoneperc, 100,",
            country)
        self.assertIn(
            "upgplace+'.24', 1, gc_upg_tooltiptype_none, gc_upg_type_effectstoneperc, 100,",
            country)
        self.assertNotIn(
            "upgplace+'.24', 1, gc_upg_tooltiptype_none, gc_upg_type_effectstoneperc, 200,",
            country)
        self.assertIn("OVDIAG STONE_CART_CFG upgrade=24 bonus=100", country)

    def test_cavalry_and_spawn_tracing(self):
        initial = self.read("scripts/units/unit.inc/initial.inc")
        tick = self.read("scripts/units/unit.inc/nothing.inc")
        self.assertIn("OVDIAG CONFIG_SPAWN", initial)
        self.assertIn("OVDIAG CAV_INIT", initial)
        self.assertIn("SetGameObjectMyTrackPointUseAnimationCycles(false)", initial)
        self.assertIn("SetGameObjectMyTrackPointMoveStep(0.045*arg_obj.cavalrySpeedFraction)", initial)
        self.assertIn("accelTime=1 movementMode=position", initial)
        self.assertIn("cavStartTime : Float = 1;", tick)
        self.assertIn("cavalrySpeedFraction+MaxFloat(0, deltatime)*0.50/cavStartTime", tick)
        self.assertIn("0.045*arg_obj.cavalrySpeedFraction*arg_obj.cavalryChargeBoost", tick)
        self.assertIn("_misc_MakeListObjectsInRadius(cavX, cavZ, 8.4,", tick)
        self.assertIn("cavDesiredBoost := 1.5;", tick)
        self.assertNotIn("cavDesiredBoost := 2.5;", tick)
        self.assertIn("arg_obj.cavalryChargeBoost+MaxFloat(0, deltatime)*0.5", tick)
        self.assertIn("chargeCap=1.5 chargeRadius=8.4", initial)
        self.assertIn("cavBoostWas<1.45", tick)
        self.assertIn("OVDIAG CAV_STATE", tick)
        self.assertIn("GetGameObjectMyPositionX", tick)
        for marker in ("OVDIAG CAV_STATE", "OVDIAG CAV_ENEMY",
                       "OVDIAG CAV_BOOST_START", "OVDIAG CAV_HIT_CHECK"):
            self.assertIn(marker, tick)
        self.assertNotIn("OVDIAG CONFIG sid=", initial)


class LogAnalyzerTests(unittest.TestCase):
    def test_report_handles_real_hp_and_missing_markers(self):
        sample = "\n".join((
            "LOG| OV_DIAG old placeholder",
            "LOG| OVDIAG GUN_INIT sid=cannon step=0.006",
            "LOG| OVDIAG CAV_BOOST_MAX hnd=111 sid=cuirassier boost=1.5",
            "LOG| OVDIAG HP_APPLY attacker=cuirassier target=musketeerpol "
            "requested=500000 applied=312501 hpBefore=70 hpAfter=-312431",
        ))
        report = analyze_overhaul_log.report(sample)
        self.assertIn("OVDIAG CAV_BOOST_MAX: OBSERVED 1", report)
        self.assertIn("Raw 500,000 cavalry charges with HP changes: 1", report)
        self.assertIn("OVDIAG AI_ART_LIMIT: NOT OBSERVED (unknown)", report)

    def test_real_cavalry_ground_speed_measurements(self):
        lines = []
        for t, x, boost in ((0, 0, 1), (1, 2, 1), (2, 3, 1.5), (3, 6, 1.5)):
            lines.append(
                f"LOG| OVDIAG CAV_STATE hnd=123 sid=cuirassier "
                f"moving=True baseSpeed=1 chargeSpeed={boost} "
                f"effectiveFactor={boost} step={0.045*boost} "
                f"x={x} z=0 t={t}"
            )
        result = analyze_overhaul_log.report("\n".join(lines))
        self.assertIn("REAL CAVALRY GROUND SPEED", result)
        self.assertIn("normal ~1x: mean=2.0000", result)
        self.assertIn("charge >1.3x: mean=3.0000", result)
        self.assertIn("Measured charge/normal speed ratio: 1.50x", result)


if __name__ == "__main__":
    unittest.main()
