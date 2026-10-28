# NavalRamming V4: findings from offline log 2026-10-28

## Measurements from cos_261028_163515.log
- Initialization: 40 INIT lines.
- Proximity detection: 877 NEAR lines.
- 205 nearby enemy detections; of these, 101 had moving=1.
- ALL 877 front values were zero. The V3 probe incorrectly used dirX/dirZ from GetGameObjectOrientationByHandle as hull forward.
- Closest detected enemy was at 4.573 world units; configured contact=3, candidate cutoff=3.5, so no candidate by distance.
- V3 broke after the first water unit, often processing friendlies instead of enemies.

## Source-based correction
- Original reference/scripts/lib/unit.script procedure _unit_SetDirection notes that heading is set as upX/upZ, not dirX/dirZ.
- Original script also uses GetGameObjectTransformedVirtualDirectionXByHandle and ZByHandle for facing.
- V4 uses upX/upZ for frontUp and separately logs frontVirt using virtual direction.
- V4 filters enemies before logging and does not stop upon finding a friendly unit.

## Instructions
In Git Bash, from the repository root:

    git switch feature/naval-ram-research
    git pull --ff-only origin feature/naval-ram-research
    powershell.exe -NoProfile -ExecutionPolicy Bypass -File ./tools/build_proximity_probe.ps1

Copy output files from mod/data/scripts/units/unit.inc/initial.inc and nothing.inc to the corresponding paths in Cossacks 3/mods/NavalRamming/data/scripts/units/unit.inc/.

In editor, create only TWO galleys, one for each opposing player. Disable peace mode and steer one galley toward the other. Search newest log for:
- NavalRamming INIT
- NavalRamming ENEMY_NEAR (distance, contact, frontUp, frontVirt, moving)
- NavalRamming APPROACH (target ahead, approaching within contact+3)
- NavalRamming RAM_CANDIDATE (target ahead within contact+0.5)

No ram damage is enabled, and the game-runtime behavior still needs validation. Next stages: calibrate physical contact, confirm heading, measure speed, add cooldown then damage. No multiplayer compatibility claim.
