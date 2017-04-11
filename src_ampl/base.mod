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

set NEW_COLS ordered by Reals;
# reduced cost of current solution from oracle
set Y_REDUCED_COST dimen 2 default {};
# cost of current solution from oracle
set Y_COST dimen 2 default {};
# first the column index and then the rows
set Y_ROW dimen 2 default {};

set Y_A dimen 3 default {};
