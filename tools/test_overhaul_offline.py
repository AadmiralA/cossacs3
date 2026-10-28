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

    def test_cavalry_and_spawn_tracing(self):
        initial = self.read("scripts/units/unit.inc/initial.inc")
        tick = self.read("scripts/units/unit.inc/nothing.inc")
        self.assertIn("OVDIAG CONFIG_SPAWN", initial)
        self.assertIn("OVDIAG CAV_INIT", initial)
        for marker in ("OVDIAG CAV_STATE", "OVDIAG CAV_ENEMY",
                       "OVDIAG CAV_BOOST_START", "OVDIAG CAV_HIT_CHECK"):
            self.assertIn(marker, tick)
        self.assertNotIn("OVDIAG CONFIG sid=", initial)


class LogAnalyzerTests(unittest.TestCase):
    def test_report_handles_real_hp_and_missing_markers(self):
        sample = "\n".join((
            "LOG| OV_DIAG old placeholder",
            "LOG| OVDIAG GUN_INIT sid=cannon step=0.006",
            "LOG| OVDIAG CAV_BOOST_MAX hnd=111 sid=cuirassier boost=2.5",
            "LOG| OVDIAG HP_APPLY attacker=cuirassier target=musketeerpol "
            "requested=500000 applied=312501 hpBefore=70 hpAfter=-312431",
        ))
        report = analyze_overhaul_log.report(sample)
        self.assertIn("OVDIAG CAV_BOOST_MAX: OBSERVED 1", report)
        self.assertIn("Raw 500,000 cavalry charges with HP changes: 1", report)
        self.assertIn("OVDIAG AI_ART_LIMIT: NOT OBSERVED (unknown)", report)


if __name__ == "__main__":
    unittest.main()
