# modularity bipartite : 
# (1/m) sum{r,b} (x[r,b]-d[r]*d[b]/m)
# pour C : (1/m) card(e in C) - (1/m)² d[R]*d[B]

set COLS default {} ordered by Reals;

#set SET_PARTITIONNING_V_COL_VAL dimen 3 default {};
#set SET_PARTITIONNING_V := union{(i,j,k) in SET_PARTITIONNING_V_COL_VAL}{i};

param POOL_SIZE default 0;
# id - cost - reduced cost
set POOL_COST dimen 3 default {};
# id - v
set POOL_SOLUTION dimen 2 default {};

set ALL_COST dimen 3 default {};
set ALL_COLUMNS dimen 2 default {};

param USE_STAB default 0;

set TOTO_COST dimen 3 default {};
set TOTO_COLUMNS dimen 2 default {};

param USE_IPS default 0;



