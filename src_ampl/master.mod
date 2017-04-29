
param N_BRANCHES := if USE_STAB == 0 then 0 else 6;
param WIDTH default 0*1e-4;
param PARAM_PENALTY default 1e-4;

param CENTER{v in V} default if USE_STAB == 1 then 0 else 1;

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

param FEAS_V{V} default 0;
param FEAS_ERROR := max{v in V}abs(FEAS_V[v]);

param OPT_ERROR default 0;
param FAKE_RHS default card(V);


param BUNDLE_STEP symbolic default 'UNKNOWN';

param Z_IS_FIXED default 0;

param Z_COST{i in 1..N_BRANCHES} := PARAM_PENALTY * 2^i;

param Z_WIDTH{i in 1..N_BRANCHES} := WIDTH*i;

param Z_NEG_FACTOR{V} default 1;
param Z_POS_FACTOR{V} default 1;

problem master;

var z{v in V: USE_STAB==2};
var z_pos{v in V, i in 1..N_BRANCHES: USE_STAB==1} >= 0;
var z_neg{v in V, i in 1..N_BRANCHES: USE_STAB==1} >= 0;

subject to dual_neg{v in V, i in 1..N_BRANCHES: USE_STAB==1}: z_neg[v, i] <= Z_NEG_FACTOR[v] * Z_COST[i];
subject to dual_pos{v in V, i in 1..N_BRANCHES: USE_STAB==1}: z_pos[v, i] <= Z_POS_FACTOR[v] * Z_COST[i];


var x{col in COLS} >= 0;

var c_dot_x = +sum{(col, cost, rc) in ALL_COST}cost*x[col];

maximize MASTER_OBJ:
	+sum{(col, cost, rc) in ALL_COST}cost*x[col]
		
	+sum{v in V, i in 1..N_BRANCHES: USE_STAB==1}z_neg[v, i]*(+CENTER[v]+Z_WIDTH[i])
	+sum{v in V, i in 1..N_BRANCHES: USE_STAB==1}z_pos[v, i]*(-CENTER[v]-Z_WIDTH[i])
	
	-sum{v in V: USE_STAB==2}0.5*(1/PARAM_PENALTY)*(CENTER[v]-z[v])^2;

subject to set_partitionning{v in V}:
	+sum{(col, v) in ALL_COLUMNS}x[col]
	#+z[v]	
	+sum{i in 1..N_BRANCHES: USE_STAB==1}z_neg[v, i]
	-sum{i in 1..N_BRANCHES: USE_STAB==1}z_pos[v, i]	
	+(if USE_STAB==2 then z[v])
	= 
	1;

subject to convexity:sum{col in COLS: USE_STAB==1} x[col] <= FAKE_RHS;

problem DEFAULT;

set PHI_SCORE dimen 2 default {};
