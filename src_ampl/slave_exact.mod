
param DUAL_FOR_RC{V};

problem slave_exact;

var y{V} binary;

var w{R,B} >= 0;

subject to ctr_UU{r in R, b in B}: w[r,b] - y[r] - y[b+MAX_R] + 1 >=0;
subject to ctr_LU{r in R, b in B}: w[r,b] - y[b+MAX_R] <=0;
subject to ctr_UL{r in R, b in B}: w[r,b] - y[r] <=0;

var slave_obj = +sum{r in R, b in B}(( if (r,b) in E then 1 else 0)-D_R[r]*D_B[b]*inv_m)*w[r,b]*inv_m;

maximize reduced_cost: slave_obj - sum{v in V}DUAL_FOR_RC[v]*y[v];