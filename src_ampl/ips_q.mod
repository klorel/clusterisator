set BASIS default {};
set P default {};
set Z default {};

set COL_X dimen 2;

set A_B := setof{x_b in BASIS, (x_b, v) in ALL_COLUMNS}(v, x_b);
set A_P := setof{x_p in P, (x_p, v) in ALL_COLUMNS}(v, x_p);
set A_Z := setof{x_z in Z, (x_z, v) in ALL_COLUMNS}(v, x_z);

set NZ_Q dimen 2 default {};

set Q   dimen 3 default {};
set Q_Z dimen 3 default {};
set Q_P dimen 3 default {};

set Q_Z_COL dimen 3;

set COL_COMPATIBLE dimen 3 default {};

problem Q_BUILDER;

var q_P{P, V};
var q_Z{Z, V};

minimize ZERO: 0;

# A A_1 = I
subject to identity{v1 in V, v2 in V}:
	# ApQp+AzQp
	+sum{(v1, x_p) in A_P}q_P[x_p, v2]
	+sum{(v1, x_z) in A_Z}q_Z[x_z, v2]
	=
	if (v1==v2) then 1 else 0;
	
	# compatiblity matrix, 
# - x_z row 
# - col
# - Q[x_z] A[col]
set Q_A_COL_VAL dimen 3 default {};