# column ID;
problem master;

#param FEAS_V{SET_PARTITIONNING_V} default 0;
#param FEAS_ERROR := max{v in SET_PARTITIONNING_V}abs(FEAS_V[v]);

var x{COLS} >= 0;

maximize MASTER_OBJ:
	+sum{col in COLS}COST[col]*x[col]
	;

subject to set_partitionning{v in V}:
	+sum{(col, v) in ALL_COLUMNS}x[col]
	= 
	1;

problem DEFAULT;