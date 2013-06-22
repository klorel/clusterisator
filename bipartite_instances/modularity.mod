reset;
param ZERO = 1e-4;
param nR integer > 0;
param nB integer > 0;


set V1 := 1..nR cross 1..nB ;
set V2 := V1 cross V1;
set V3 := V1 cross 1..nB ;

set E within {(i,j) in V1 };

param a{(i,j) in V1} :=  if ((i,j) in E ) then 1.0 else 0.0;

param kR{ i in 1..nR } := card({(i,j) in E});
param kB{ j in 1..nB } := card({(i,j) in E});

param m := 1.0*card(E);
param inv_m := 1.0/m;

param w{(i,j) in V1 } := (a[i,j]-kR[i]*kB[j]*inv_m)*inv_m;
