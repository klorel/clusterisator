
param M_INTEGER default 0;

# column ID;
problem master;

var x{COLS} >= 0;

maximize MASTER_OBJ: sum{col in COLS}COST[col]*x[col];

subject to ctr{row in ROWS}:sum{(row, col) in ROW_COL}A[row, col]*x[col] = RHS[row];