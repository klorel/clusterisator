
param DUAL_FOR_RC{V} default 0;
param PI_STABILISED{V} default 0;

param T_R := ceil(1+log(sum{r in R}D_R[r])/log(2))-1;
param T_B := ceil(1+log(sum{b in B}D_B[b])/log(2))-1;


set CHECKER_V;
param CHECKER_Y{v in V} := if v in CHECKER_V then 1 else 0;
param CHECKER_COST := +sum{r in R, b in B}(( if (r,b) in E then 1 else 0)-D_R[r]*D_B[b]*inv_m)*inv_m*CHECKER_Y[r]*CHECKER_Y[b+MAX_R];
param CHECKER_REDUCED_COST := CHECKER_COST - sum{v in V}DUAL_FOR_RC[v]*CHECKER_Y[v];

problem slave_exact;

var y{V} binary;

#model slave_exact_milp.mod;
model slave_exact_binary_decomposition.mod;

maximize reduced_cost: slave_cost - sum{v in V}DUAL_FOR_RC[v]*y[v];

var slave_cost_y = +sum{r in R, b in B}(( if (r,b) in E then 1 else 0)-D_R[r]*D_B[b]*inv_m)*inv_m*y[r]*y[b+MAX_R];

var reduced_cost_y = slave_cost_y - sum{v in V}DUAL_FOR_RC[v]*y[v];

var den_phi_bar = max(1, sum{v in V}y[v]);

var phi = +slave_cost-sum{v in V}PI_STABILISED[v]*y[v];
