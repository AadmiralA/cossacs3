# NavalRamming v5 — findings and next game test

## Evidence from uploaded game log cos_261028_164215.log

The diagnostic v4 successfully ran in Cossacks 3, without a script compilation error:
- LoadFromStreamProject: 1:28.246, unaffected long game startup issue.
- Two galley INIT messages.
- Fourteen ENEMY_NEAR samples, with seven moving=1.
- The closest observed enemy separation was approximately 0.960 world units.
- During closing: dist=2.943836, contact=3, moving=1, frontUp=-0.945899, frontVirt=+0.945900.
- The existing v4 candidate check tested frontUp>0.65, which falsely rejected this true approach.
- When distance later reached 3.790811 while moving=1, frontUp=+0.978548 and frontVirt=-0.978549; v4 emitted APPROACH *while moving away*, another example of backwards heading.

## Change made in v5

The game log empirically demonstrates that *transformed virtual direction* follows the attacking galley's course during this test, while the up direction is reversed relative to forward travel. The generator now uses frontVirt>0.65 (rather than frontUp>0.65) for APPROACH and RAM_CANDIDATE while continuing to log both headings for comparison.

For the same measured samples, the expected v5 results are:
- APPROACH at dist approximately 2.94
- RAM_CANDIDATE at dist approximately 2.94 (contact+0.5 = 3.5)
- **No false approach** at dist approximately 3.79 when frontVirt is negative.

These are predicted results from the provided log, not verified engine output from v5.

## Build / deploy

Git Bash, on branch feature/naval-ram-research:

    git pull --ff-only origin feature/naval-ram-research
    powershell.exe -NoProfile -ExecutionPolicy Bypass -File ./tools/build_proximity_probe.ps1

Copy both generated files:
- mod/data/scripts/units/unit.inc/initial.inc
- mod/data/scripts/units/unit.inc/nothing.inc

into the enabled Cossacks 3 mod at mods/NavalRamming/data/scripts/units/unit.inc/.

Create 2 enemy galleys, disable peace mode, order one to move straight through or into the other. Inspect log markers ENEMY_NEAR, APPROACH, RAM_CANDIDATE.

No damage enabled yet. After confirming v5 candidate detection, implement damage with proper per-ship cooldown, motion/contact checks, server/offline constraints, and safe load/save behavior. Do not implement direct HP decrements that bypass game combat/death logic.
