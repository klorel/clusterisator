
param M_INTEGER default 0;
param M_TIME default 0;
# column ID;
problem master;

param N_BRANCHES := if USE_STAB == 0 then 0 else 6;
param WIDTH default 1e-4;
param PARAM_PENALTY default 0.5;

param CENTER{v in SET_PARTITIONNING_V} default 0;
param PI{v in SET_PARTITIONNING_V} default 0;

param PHI_PI default 0;
param PHI_B_PI default 1e20;
# the current value
param PHI_PI_BAR default 1e20;

param PI_DOT_B default 0;

param C_DOT_X default 0;

param SCORE;


param AT_LEAST_ONE_SS default 0;

param M := 0.1;
param NB_NS default 0;
param NB_SS default 0;

param FEAS_V{SET_PARTITIONNING_V} default 0;
param FEAS_ERROR := max{v in SET_PARTITIONNING_V}abs(FEAS_V[v]);

param OPT_ERROR := abs(C_DOT_X - PHI_PI_BAR);



param BUNDLE_STEP symbolic default 'UNKNOWN';

param Z_IS_FIXED default 0;

param Z_COST{i in 1..N_BRANCHES} := PARAM_PENALTY * 2^i;

param Z_WIDTH{i in 1..N_BRANCHES} := WIDTH*i;

var z_pos{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES} >= 0, <= Z_COST[i];
var z_neg{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES} >= 0, <= Z_COST[i];

var x{COLS} >= 0;

var c_dot_x = +sum{col in COLS}COST[col]*x[col];

maximize MASTER_OBJ:
	+sum{col in COLS}COST[col]*x[col]
	+sum{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES}z_neg[v, i]*(CENTER[v]-Z_WIDTH[i])
	-sum{v in SET_PARTITIONNING_V, i in 1..N_BRANCHES}z_pos[v, i]*(CENTER[v]+Z_WIDTH[i])
	;

subject to set_partitionning{v in SET_PARTITIONNING_V}:
	+sum{(v, col, val) in SET_PARTITIONNING_V_COL_VAL}val*x[col]
	+sum{i in 1..N_BRANCHES}z_neg[v, i]
	-sum{i in 1..N_BRANCHES}z_pos[v, i]	
	= 
	1;

subject to fake_constraint{i in {USE_STAB}: USE_STAB==1}: sum{(col, val) in FAKE_COL_VAL}val*x[col] <= FAKE_RHS;