
param M_INTEGER default 0;
param M_TIME default 0;
# column ID;
problem master;

param N_BRANCHES := if USE_STAB == 0 then 0 else 2;
param WIDTH := 1e-3;
param PARAM_PENALTY default 4;

param CENTER{v in SET_PARTITIONNING_V} default 0;
param PI{v in SET_PARTITIONNING_V} default 0;

param PHI_PI default 0;
param PHI_B_PI default 1e20;
# the current value
param PHI_PI_BAR default 1e20;

param PHI_DOT_B default 0;

param C_DOT_X default 0;

param SCORE;

param M := 0.1;
param NB_NS default 0;
param NB_SS default 0;

param FEAS_V{SET_PARTITIONNING_V} default 0;
param FEAS_ERROR := max{v in SET_PARTITIONNING_V}abs(FEAS_V[v]);

param BUNDLE_STEP symbolic default 'UNKNOWN';

var z_pos{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES} >= 0, <= (1/PARAM_PENALTY)^(N_BRANCHES-i);
var z_neg{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES} >= 0, <= (1/PARAM_PENALTY)^(N_BRANCHES-i);

var x{COLS} >= 0;

var c_dot_x = 	+sum{col in COLS}COST[col]*x[col];

maximize MASTER_OBJ:
	+sum{col in COLS}COST[col]*x[col]
	+sum{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES}z_neg[v, i]*(CENTER[v]-WIDTH*i)
	-sum{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES}z_pos[v, i]*(CENTER[v]+WIDTH*i)
	;

subject to set_partitionning{v in SET_PARTITIONNING_V}:
	+sum{(v, col, val) in SET_PARTITIONNING_V_COL_VAL}val*x[col]
	+sum{i in 1..N_BRANCHES}z_neg[v, i]
	-sum{i in 1..N_BRANCHES}z_pos[v, i]	
	= 
	1;

subject to fake_constraint: sum{(col, val) in FAKE_COL_VAL}val*x[col] <= FAKE_RHS;