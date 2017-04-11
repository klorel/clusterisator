# modularity bipartite : 
# (1/m) sum{r,b} (x[r,b]-d[r]*d[b]/m)
# pour C : (1/m) card(e in C) - (1/m)² d[R]*d[B]



set ROWS default {} ordered by Reals;
param RHS {ROWS} := 1;
param DUAL{ROWS} default 0;

set COLS default {} ordered by Reals;
param NEXT_ID := 1+card(COLS);
param NEW_ID := NEXT_ID-1;

param COST{COLS};
param PRIMAL{COLS};

set ROW_COL dimen 2 default {};
param A{ROW_COL} default 0;


# current solution from oracle
param Y_COST;

set Y_ROW default {};

set Y_A dimen 2 default {};
