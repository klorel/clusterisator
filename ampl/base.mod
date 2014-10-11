
display instance, chosen_formulation;
# exit;
suffix lazy IN;
# Attention IJ contient des paires (i,j) avec i et j libre
set IJ dimen 2;

set I := setof{(i,j) in IJ}i;
set J := setof{(i,j) in IJ}j;

set R := I;
param p := max{i in I}i;
set B := setof{j in J}(j+p);

set E := setof{(i,j) in IJ}(i, j+p);

set V := R union B;
set V2 := {i in V, j in V : i<j};
set V3 := {(i,j) in V2, k in V : j<k};

param n := card(V);
param m := card(E);

set Neighbor{u in V} := { (u,v) in E } union { (v,u) in E};

param d{u in V} := card(Neighbor[u]);
set Nm{(i,j) in V2} :=  
	(if (d[i]>d[j]) then {Neighbor[j] diff {i}} else {Neighbor[i] diff {j}});
	# (Neighbor[j] union Neighbor[i]) diff {i,j};

param A{(r,b) in V2} := if b in Neighbor[r] then 1 else 0;
param H{(r,b) in V2} := A[r,b]-d[r]*d[b]/m;

# subject to reflexivity{i in V, j in V diff{i}}: t[i,j] = t[j,i];
# subject to ij_jk_ik{(i,j) in V2, k in V: i<j<k}: +t[i,j]+t[j,k]-t[i,k]<=1;
# subject to ij_ik_jk{(i,j) in V2, k in V: i<j<k}: +t[i,j]-t[j,k]+t[i,k]<=1;
# subject to jk_ik_ij{(i,j) in V2, k in V: i<j<k}: -t[i,j]+t[j,k]+t[i,k]<=1;


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