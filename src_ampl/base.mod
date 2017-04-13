# modularity bipartite : 
# (1/m) sum{r,b} (x[r,b]-d[r]*d[b]/m)
# pour C : (1/m) card(e in C) - (1/m)� d[R]*d[B]

set COLS default {} ordered by Reals;
param NEXT_ID := 1+card(COLS);
param NEW_ID := NEXT_ID-1;

param COST{COLS};
param PRIMAL{COLS};

set FAKE_COL_VAL dimen 2 default {};
set SET_PARTITIONNING_V_COL_VAL dimen 3 default {};
set SET_PARTITIONNING_V := union{(i,j,k) in SET_PARTITIONNING_V_COL_VAL}{i};

# id - cost - reduced cost
set POOL_COST dimen 3 default {};
# id - v
set POOL_SOLUTION dimen 2 default {};

set ALL_COST dimen 3 default {};
set ALL_SOLUTION dimen 2 default {};

param FAKE_RHS := card(SET_PARTITIONNING_V);

param USE_STAB := 1;