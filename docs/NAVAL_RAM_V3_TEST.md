# NavalRamming V3 — proximity probe

**Status:** untested in Cossacks 3, NO DAMAGE. This probe replaces dependence on the native ship collision callback with a proximity query in the server-side unit progress handler.

## Source findings

- The previous test printed repeated NavalRamming INIT for galley, proving that the override of units/unit.inc/initial.inc was active.
- The same test printed neither NavalRamming CALLBACK nor NavalRamming PROBE after two galleys were steered into one another. We have NOT established that _misc_Collided is called on ships.
- units/unit.inc/nothing.inc calls _misc_DoProgressOrders on each unit tick in its non-client path.
- lib/misc.script exposes _misc_MakeListObjectsInRadius(x, y, r, bcheckdist, mmask, needcount, bAddRadius, list).
- lib/unit.script exposes _unit_GetTObjFast; the engine exposes GetGameObjectOrientationByHandle and GetGameObjectMyTrackPointMovementModeInt.

## Run on Windows

In Git Bash from your local repository, while on branch feature/naval-ram-research:

    git pull --ff-only origin feature/naval-ram-research
    powershell.exe -NoProfile -ExecutionPolicy Bypass -File ./tools/build_proximity_probe.ps1

This produces two output files:

    mod/data/scripts/units/unit.inc/initial.inc
    mod/data/scripts/units/unit.inc/nothing.inc

Copy BOTH files to your active Cossacks 3 mod at:

    mods/NavalRamming/data/scripts/units/unit.inc/

You can keep the previously deployed lib/miscext.script temporarily; its old callback logging is not used by the new detector. For cleaner diagnostics, remove that old **mod** copy only (never delete the game's original data/scripts/lib/miscext.script).

Start the game in offline editor mode, turn peace mode off, place only two enemy galleys, give one an ordinary movement order toward the other. Collect a new log.

## Meaning of log markers

- NavalRamming INIT: a water unit was initialized using the overridden file.
- NavalRamming NEAR: a galley found any nearby water unit within 12 engine world units. This line prints dist, contact (sum of defined radii), front (dot product with target vector; positive means the target is ahead), moving (movement mode int), and enemy (1 for hostile, 0 otherwise).
- NavalRamming RAM_CANDIDATE: a galley scanned a hostile water unit while moving, the target lay toward the bow, and distance was within the approximate contact threshold.

## Limitations

- This is a deliberately **safe diagnostic**, not damage or confirmed physical contact.
- Scan is limited to galleys in an offline game, once per 32 unit ticks, so a fast brief contact can be missed.
- Uses a shared temporary nearby-objects list; no nesting/online correctness guarantees. Avoid other mods altering this part of the unit tick. Do not use for multiplayer.
- The engine's water-unit collision radii may not match actual ship hull contact. Use measured log distances to tune the threshold before enabling damage.
- This implementation and the generator have been checked against committed source anchors but **not compiled or tested by the Cossacks 3 runtime**.
- Next stage after confirming RAM_CANDIDATE: implement ram damage through _misc_DoDamage with per-ship cooldown, angle/speed checks, and rebound/self-damage; verify save/load and networking separately.
