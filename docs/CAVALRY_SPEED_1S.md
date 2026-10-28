# Cavalry speed hotfix — one-second acceleration

**Branch:** `feature/gameplay-overhaul-phase1`. Scope: cavalry only; guns and ships unchanged.

## Why cavalry felt slow

Original `reference/objects/ref/refunit.prop` defines `MoveStep=0.03`. The phase-1 cavalry prototype took only 35% of this on spawn (`0.0105`), then grew back to merely `0.03` over **4 seconds for light cavalry and 6 for heavy cavalry**. It set `MoveStep` dynamically, but `reference/scripts/units/unit.inc/initial.inc` initializes **`SetGameObjectMyTrackPointUseAnimationCycles(true)`**, so in-game movement can remain controlled by animation cycles rather than the changing MoveStep. Log markers `CAV_BOOST_MAX` and `baseSpeed=1` do NOT prove real ground speed changes.

## What is changed

- Acceleration **for every cavalry type**: **0.50x → 1.00x in 1 unit of game-time**. Starts at 50% rather than 35% for a less sluggish start.
- Normal fully-accelerated cavalry movement step **0.045**, up **50%** from the old phase-1 limit 0.03. Initial step 0.0225, not 0.0105.
- Mounted units only: disable track-point use of animation cycles (`SetGameObjectMyTrackPointUseAnimationCycles(false)`) after original initialization, so dynamic movement steps can drive real translation. This is **experimental**; test movement, visuals, collisions and formation integrity.
- Formed heavy cavalry approaching a non-pike enemy: temporary 2.5x normal speed boost still applies. Charge boost 1.0→2.5 now rises over approximately **1 game-time unit** rather than three. Intended effective maximum step is `0.045 × 2.5 = 0.1125` — NOT confirmed in-engine.
- `OVDIAG CAV_STATE` now logs `hnd`, `x`, `z`, `t`, movement `step` and `effectiveFactor`. `Full` mode records every 8 progress ticks, enough to observe acceleration; `Compact` remains every 64 ticks.
- `tools/analyze_overhaul_log.py` calculates observed coordinate distance per unit of game time for initial/normal/charging horses and compares the latter two. Values are approximate; path turns, collisions and formation congestion can reduce true displacement.
- Pre-deployment checks in `tools/overhaul_oneclick.ps1` and CI regression checks protect against losing the speed changes in future versions.

## Test

On a new OFFLINE map place one horse on a straight, long route. Observe whether it reaches normal top speed in ~1 unit of game-time; then use a formed heavy cavalry squad against non-pike infantry and watch the charge. In crowded formations actual translation may be below configured speed. Check visible horse leg animations and avoid using in multiplayer until validated.

~~~bash
cd /d/Work/Cossacs3-mod/cossacs3
git switch feature/gameplay-overhaul-phase1
git pull --ff-only origin feature/gameplay-overhaul-phase1
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ./tools/overhaul_oneclick.ps1 -Update -Mode Full
~~~

For an offline-only build without game launch, use `-NoLaunch`. The launcher automatically copies all eight overrides. If game path has not been set up yet, supply `-GameRoot "C:\path\to\Cossacks 3"`.

After the game closes, upload `cos_*.log` or `overhaul_report.txt` from `%LOCALAPPDATA%\Cossacks3Overhaul\runs\<timestamp>\`. Look for **REAL CAVALRY GROUND SPEED**, not only `OVDIAG CAV_BOOST_MAX`.

## Validation status

This code has been edited in GitHub and is statically checked. **The actual physical movement in the game and the effect of disabling animation-cycle movement have not yet been verified.** Restore the backup from the latest run if horses stop moving or their animations become broken. The old max 0.03 and 4–6 second ramp can be restored from earlier commits.
