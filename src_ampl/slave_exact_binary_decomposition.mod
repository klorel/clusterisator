
var slave_d_r;
var slave_d_b;

var slave_a{0..T_R} binary;
var slave_b{0..T_B} binary;
var slave_a_b{0..T_R, 0..T_B} >= 0;

subject to d_r_constraint_1: slave_d_r = sum{r in R}D_R[r]*y[r];
subject to d_b_constraint_1: slave_d_b = sum{b in B}D_B[b]*y[b+MAX_R];

subject to d_r_constraint_2: slave_d_r = sum{h in 0..T_R}(2^h)*slave_a[h];
subject to d_b_constraint_2: slave_d_b = sum{l in 0..T_B}(2^l)*slave_b[l];

subject to fortet_1{h in 0..T_R, l in 0..T_B}: slave_a_b[h,l] >= slave_a[h]+slave_b[l]-1;
subject to fortet_2{h in 0..T_R, l in 0..T_B}: slave_a_b[h,l] <= slave_a[h];
subject to fortet_3{h in 0..T_R, l in 0..T_B}: slave_a_b[h,l] <= slave_b[l];

var w{E} >= 0;

subject to ctr_UU{(r,b) in E}: w[r,b] - y[r] - y[b+MAX_R] + 1 >=0;
subject to ctr_LU{(r,b) in E}: w[r,b] - y[b+MAX_R] <=0;
subject to ctr_UL{(r,b) in E}: w[r,b] - y[r] <=0;


var slave_cost = 
	+inv_m*sum{(r,b) in E}w[r,b]
	-inv_m*inv_m*sum{h in 0..T_R, l in 0..T_B}(2^(l+h))*slave_a_b[h,l];


