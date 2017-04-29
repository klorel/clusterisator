set BASIS default {};
set P default {};
set Z default {};

set COL_X dimen 2;

set NZ_Q dimen 2 default {};

set Q   dimen 3 default {};
set Q_Z dimen 3 default {};
set Q_P dimen 3 default {};

set Q_Z_COL dimen 3;

set COL_COMPATIBLE dimen 3 default {};

problem Q_BUILDER;

var q_builder{V, BASIS};

minimize least_square: sum{(col,val) in COL_X}(val-sum{v in V}q_builder[v, col])^2;

# A A_1 = I
subject to identity{v1 in V, v2 in V}:
	sum{(col, v1) in ALL_COLUMNS: col in BASIS} q_builder[v2, col]
	=
	if (v1==v2) then 1 else 0;