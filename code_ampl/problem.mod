
display instance, criterion, chosen_formulation;
# exit;
suffix lazy IN;
# Attention IJ contient des paires (i,j) avec i et j libre

param UNIPARTITE symbolic := "UNIPARTITE";
param BIPARTITE  symbolic := "BIPARTITE";

set IJ dimen 2;

set I := setof{(i,j) in IJ}i;
set J := setof{(i,j) in IJ}j;


set R := I;
param p := if criterion == BIPARTITE then( max{i in I}i) else( 0 );
set B := setof{j in J}(j+p);
set E := if criterion == BIPARTITE then(setof{(i,j) in IJ}(i, j+p))else( IJ );

set V := if criterion == BIPARTITE then( R union B) else( I union J);
set V2 := {i in V, j in V : i<j};
set V3 := {(i,j) in V2, k in V : j<k};

set EDGE_COST := if criterion == BIPARTITE then(R cross B)else( V2 );

param n := card(V);
param m := card(E);

set Neighbor{u in V} := { (u,v) in E } union { (v,u) in E};
param d{u in V} := card(Neighbor[u]);

param C := if criterion == BIPARTITE then 0 else( sum{v in V}d[v]*d[v]/(2*m)^2 );

param A{(r,b) in V2} := if b in Neighbor[r] then 1 else 0;
param H_FACTOR := if criterion == BIPARTITE then m else 2*m;
param H{(r,b) in V2} := (A[r,b]-d[r]*d[b]/H_FACTOR)/m;


param return_code		 default -1;
param best_modularity	 default -1;
param number_of_nodes	 default -1;
param real_time			 default -1;
param transitivity_viol	 default -1;
param nb_constraints	 default -1;
param nb_variables		 default -1;
param nb_cluster		 default -1;

param cluster{V};

param t_value{V2} default 0;
param together{u in V, v in V} := if u==v then 1 else (if u<v then t_value[u,v] else t_value[v,u]);

set together_set{u in V} := { v in V diff {u}: together[u,v] == 1};


set FORMULATIONS := {
	"CPI", 
	"CPI_varneg",
	"CPS2"
};

set CONSTRAINT_FOR_PROBLEM{formulation in FORMULATIONS, kind in 1..3} dimen 3 :=
  if formulation == "CPI" then(
		V3
	)else	if formulation == "CPI_varneg" then(
		{(i,j) in V2, k in V: i<j<k}
	)else if formulation == "CPS2" then(
		if kind == 1 then 
			{(i,j,k) in V3: ( H[i,j]>=0 or H[j,k]>=0)}
		else if kind == 2 then 
			{(i,j,k) in V3: ( H[i,j]>=0 or H[i,k]>=0)}
		else if kind == 3 then 
			{(i,j,k) in V3: ( H[j,k]>=0 or H[i,k]>=0)}
		else
			{(1/0,0,0)}
	)else(
		{(1/0,0,0)}
	)
;


###
var t{V2} binary;

subject to constraints{kind in 1..3, (i,j,k) in V3}: 
	+(if kind == 3 then -1 else 1)*t[i,j]
	+(if kind == 2 then -1 else 1)*t[j,k]
	+(if kind == 1 then -1 else 1)*t[i,k]
	<=1;


maximize modularity: -C+sum{(i,j) in EDGE_COST}H[i,j]*t[i,j];

