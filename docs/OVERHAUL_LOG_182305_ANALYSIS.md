# Gameplay Overhaul: audit of cos_261028_182305.log

Date: 2026-10-28. This is a **runtime audit**, not a claim that subsequent source patches have been game-tested.

## Overall
- 2,541 lines (263,424 bytes), down from 24,294 lines in the prior diagnostic test.
- Game load stage: 62.225 seconds (LoadFromStreamProject).
- No new Pascal script compilation/crash messages in the log; preexisting FileStreamExists and Steam warnings remain.
- 23 MORTAR_MEMBER_DISABLED markers; registry hook ran, not proof of removal from editor/scenarios.
- CONFIG_SPAWN: one cannon; GUN_INIT: one cannon at step **0.018** in this build. There is no spawn-config for the already existing tower, but the tower fired.

## Evidence by mechanic
| Mechanic | Count/evidence | Status |
| --- | --- | --- |
| Polish musketeers formed | 476 RELOAD, pause=3.125 | Confirmed |
| Polish musketeers unformed | 52 RELOAD, pause=4.8076925 | 0.65x firing rate, confirmed |
| Bullet damage at source | 9 -> 6 DAMAGE_GLOBAL | global x0.625 confirmed |
| Normal bullet vs armored vityaz, formed | 5 HP most common | observed |
| Normal bullet vs armored vityaz, unformed | 1 HP most common | **wrong effective reduction**: 80%, because 35% applied before flat armor |
| HP_APPLY | 909 events | actual HP subtraction logged |
| Horse init | 35 vityaz CAV_INIT | unformed, cavalry motor initialized at 0.35x |
| Horse state | 76 CAV_STATE; 46 NO_FORMATION blocks | movement factor varies, special charge inhibited as intended |
| Heavy cavalry x2.5 charge | 0 BOOST/MAX/HIT | **not tested**; vityaz all had squad=-1 |
| Cannon movement | 1 GUN_INIT at step=0.018 | **bug**: stock InitRollBehaviour uses 0.010, so this was a speed increase, not a slowdown |
| Cannon actual combat | 3 RELOAD, 17 HP_APPLY | Fired independently; two-person crew remains unimplemented |
| Cannon reload | all 3 RELOAD pause=10.9375; config reload0=16.40625 | **bug**: second (canister) weapon slot had not been multiplied by 1.5 |
| Tower shooting | 21 HP_APPLY from eurtow, 3 RELOAD with pauses 7.5/10 | firing confirmed, tower weapon count upgrade unimplemented |
| AI artillery cap | 0 AI_ORDER/ART_LIMIT | not tested |
| Ships | no NavalRamming entries | not tested |

The native `miscext2.script` has a 5% bullet `bHeadShot` event adding a potentially large amount based on `uniqrnd*500`. This explains why some musketeer hits with requested=9 show applied damage in the hundreds; the separate headshot logic is not caused directly by the formation penalty.

## Fixes committed after receiving THIS log
- Move the additional 35% no-formation damage debuff **after** fixed armor and formation bonuses. For normal 5-HP formed shots against the same vityaz, 35% of final damage should give ~3 HP, rather than the observed 1 HP. Main and non-logic paths updated in `tools/build_gameplay_overhaul_phase1.ps1`.
- Correct cannon/howitzer move-step override from **0.018 to 0.006** (stock is **0.010**). Same diagnostic generator anchor updated.
- Multiply *both* artillery weapon-slot reloads by 1.5 if secondary slot enabled, not only weapon[0]. The cannon canister reload (weapon[1]) should now be increased.
- Add `reload1`, unit handles and player ownership in diagnostic output to help check these behaviors.
- Log a stable per-horse handle and initial timestamp; previous CAV_STATE samples combined multiple identical vityaz and could not prove individual 4-6 second ramps.

These new changes were static-checked against the project's reference source but **not yet compiled or tested in the Cossacks 3 game engine**.

## Next test
Rebuild diagnostics using the latest phase-1 branch; deploy all 8 regenerated files to the active mod (fresh offline test map). Try one cannon firing canister, a formed squad of musketeers against an armored vityaz and one unformed musketeer firing at a similar target. Optionally add a formed heavy cavalry squad to validate boosted charge; the prior test did not exercise that case.

Unimplemented design items (two-person artillery crew, physical breakthrough of ranks, stone depletion, payroll/mutiny, new fortress, automatic retreat) are not assessed by this log.
