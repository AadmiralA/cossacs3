# Naval ramming: architecture and diagnostic test

Status: RESEARCH + DIAGNOSTIC PROTOTYPE ONLY. No damage implementation, not game-tested.

## Verified against files in reference/

1. reference/scripts/dmscript.source defines script load order. miscext.script is loaded before miscext2.script.
2. reference/scripts/units/unit.inc/initial.inc enables collision callbacks via SetGameObjectMyCollisionExecAsFunc(true), sets enemy-front rule via SetGameObjectMyRuleCollidedExecEn(2, 90.0, False) and names the callback _misc_Collided. It also configures distinct mass and collision inertia for ship types.
3. reference/scripts/lib/miscext.script implements _misc_Collided(hnd) and retrieves the collision counterpart via GetGameObjectStateCollisionObjectByHandle(hnd). It processes on offline/server, excluding replays. The existing body/body logic initiates ordinary attack commands; there is no ram damage.
4. reference/scripts/lib/unit.script defines ship properties: gc_obj_media_water, gc_obj_motionstyle_ship, individual collision radii and weapon stats. Galley has 35000 HP, frigate 50000, battleship 90000.
5. reference/scripts/lib/miscext2.script defines _misc_DoDamage(goHnd, trgHnd, indamage, weapind, weapkind) for damage processing. Because this function appears in a later script than the collision callback, direct use must be confirmed by the game's script compiler.
6. reference/scripts/dmscript.global defines TObj, with custom save serialization configured by dmscript.source. Adding persistent cooldown fields to TObj needs serialization and networking analysis.

## Collision diagnostic

The tool tools/build_collision_probe.ps1 copies the existing reference/scripts/lib/miscext.script into the **local generated** path mod/data/scripts/lib/miscext.script and inserts an observation-only test inside the collision callback. It checks that both colliders are initialized playable water units and that their owners are enemies, then calls Log with prefix NavalRamming PROBE.

This does not demonstrate real bow contact rather than engine collision-inertia proximity; run an in-game test.

### Windows instructions

Open Windows PowerShell in the repository root and run:

    powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build_collision_probe.ps1

Or in Git Bash:

    powershell.exe -NoProfile -ExecutionPolicy Bypass -File ./tools/build_collision_probe.ps1

Copy the generated mod/data/scripts/lib/miscext.script into your enabled mod preserving the relative data/scripts/lib directory. Use a short **offline** editor scenario with two enemy galleys. Drive them together and search game/editor logs for NavalRamming PROBE. Log entries can repeat frequently, so keep the diagnostic test brief.

The build script preserves original bytes where possible, does not modify reference/, and refuses to patch when its expected anchor is missing. The generated original-derived file should not be committed to a public repository.

## Next engineering tasks

- Confirm the ship collision callback occurs at relevant contact geometry, and inspect the probe logs.
- Compute bow heading versus target offset using GetGameObjectOrientationByHandle and X/Z position API. Verify orientation sign.
- Compute actual speed from a robust deterministic movement-state sample. Movement state alone is not a speed measurement.
- Add a separate per-attacker or per-pair cooldown resilient to repeated collision events, save/load and network roles.
- Apply damage via _misc_DoDamage on the authoritative simulation side after checking cross-script declaration rules.
- Test health, destruction, save/load, replay and AI. Multiplayer is not yet supported/tested.

## Project conventions and warnings

- reference/ : untouched original data for architecture study.
- mod/ : generated playable mod output.
- tools/ : generators and diagnostics.
- No modifications to cossacks.exe.
- The repository was publicly visible during review. Consider switching visibility to Private before storing additional original assets.

## Diagnostic v2 interpretation (2026-10-28)

The generator now produces TWO derived files: `mod/data/scripts/lib/miscext.script` and `mod/data/scripts/units/unit.inc/initial.inc`. Deploy both into the enabled mod.

- `NavalRamming INIT ship=galley`: the overridden initializer was loaded and a galley was created. If missing after spawning a new ship, first check enabled-mod priority and the deployed file path.
- `NavalRamming CALLBACK ship=galley`: the engine called `_misc_Collided` on a water unit. This may be reported even when the other collider is not an enemy vessel.
- `NavalRamming PROBE ship=galley target=galley`: the collision counterpart passed the existing playability checks, both were water units and they were on enemy teams according to the in-game mask.

Absence of CALLBACK with INIT present means the event did not reach the callback during the test; it is not proof that the game has no native ship collision detection. Check the collision rules, order/pathing and how the ships were steered before changing the approach. Logs can be verbose; do not leave the diagnostic enabled in normal play.

The user's latest log with the mod disabled still shows approximately 95 seconds in LoadFromStreamProject, so a mod-specific slowdown has not been demonstrated. Investigate loading time separately.
