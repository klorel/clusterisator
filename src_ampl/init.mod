set E dimen 2;

param m := card(E);
param inv_m := 1/m;

set R := union{(r,b) in E}{r} ordered by Reals;
set B := union{(r,b) in E}{b} ordered by Reals;

param D_R{r in R} := card({(r,b) in E});
param D_B{b in B} := card({(r,b) in E});

param MAX_R := 1+max{r in R}r;

set V := R union setof{b in B}(MAX_R+b) ordered by Reals;

param D{v in V} := if v<=MAX_R then D_R[v] else D_B[v-MAX_R];

param Y{V};