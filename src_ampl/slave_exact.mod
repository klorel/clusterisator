
param DUAL_FOR_RC{V} default 0;

problem slave_exact;

var y{V} binary;

var w{R,B} >= 0;

subject to ctr_UU{r in R, b in B}: w[r,b] - y[r] - y[b+MAX_R] + 1 >=0;
subject to ctr_LU{r in R, b in B}: w[r,b] - y[b+MAX_R] <=0;
subject to ctr_UL{r in R, b in B}: w[r,b] - y[r] <=0;

var slave_cost = +sum{r in R, b in B}(( if (r,b) in E then 1 else 0)-D_R[r]*D_B[b]*inv_m)*w[r,b]*inv_m;

maximize reduced_cost: slave_cost - sum{v in V}DUAL_FOR_RC[v]*y[v];

var slave_cost_y = +sum{r in R, b in B}(( if (r,b) in E then 1 else 0)-D_R[r]*D_B[b]*inv_m)*inv_m*y[r]*y[b+MAX_R];
var reduced_cost_y = slave_cost_y - sum{v in V}DUAL_FOR_RC[v]*y[v];