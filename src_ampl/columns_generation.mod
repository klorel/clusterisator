
param CG_ITE   default 0;

param CG_STOP  default 0;

param CG_COLS := card(COLS);

param CG_BOUND default 0;

param CG_REDUCED_COST := if card(POOL_COST) == 0 then 0 else max{(__id, __cost, __rc) in POOL_COST}__rc;

param ZERO_REDUCED_COST := 1e-6;

param CG_SUCCESS := if CG_REDUCED_COST > ZERO_REDUCED_COST then 1 else 0;

param CG_ADDED default 0;

param HEUR_NUM default 0;

param STEP symbolic default "";
