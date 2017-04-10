# modularity bipartite : 
# (1/m) sum{r,b} (x[r,b]-d[r]*d[b]/m)
# pour C : (1/m) card(e in C) - (1/m)² d[R]*d[B]



set ROWS default {};
param RHS {ROWS} := 1;
param DUAL{ROWS} default 0;

set COLS default {};
param NEXT_ID := 1+card(COLS);
param NEW_ID := 1-NEXT_ID;

param COST{COLS};
param PRIMAL{COLS};

set ROW_COL dimen 2 default {};
param A{ROW_COL} default 0;


# current solution from oracle
param Y_COST;

set Y_ROW default {}
;
param Y_A{Y_ROW};
