# Gameplay overhaul diagnostic log audit

Source: cos_261028_180621.log, 24,294 lines.

## Verified
- 16 cavalry hits against isolated musketeers with confirmed HP reductions in HP_APPLY.
- Heavy cavalry multiplier reaches 2.5: 124 BOOST_MAX markers.
- 154 pike-related special-attack blocks.
- 3 no-formation blocks.
- 1 confirmed ship ram: real HP loss of 3,170.
- Galley acceleration: 195.534 to 199.496 game-time units.
- The galley's second weapon slot is disabled in its startup configuration.
- 23 country mortar exclusion markers.

## Not verified
- Gun movement: no GUN_INIT markers.
- Bot artillery quota: no AI_ORDER or AI_ART_LIMIT.
- Land reload modifiers: only a galley RELOAD line.
- Physical pushback, two-person crews, stone depletion, fortress, payroll, mutiny, starvation and rout remain unimplemented.

## Diagnostic defects corrected
- 18,600 duplicate CONFIG entries caused by template initialization; replaced with CONFIG_SPAWN on actual object initialization.
- Ships and artillery were inadvertently slowed by the no-formation 35% attack penalty. Exempted in the stage-one generator.
- The analyzer will now report confirmed HP results and noisy legacy configuration lines.

New generator must be compiled and tested in the game before these corrections can be considered verified.