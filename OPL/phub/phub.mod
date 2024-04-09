/*********************************************
 * OPL 12.9.0.0 Model
 * Author: jardell
 * Creation Date: 10 de set de 2020 at 23:46:45
 *********************************************/
int N = ...;
int p = ...;
float alpha = 0.75;
float betha = 3.0;
float gama = 2.0;
float c[1..N][1..N] = ...;
float w[1..N][1..N] = ...;
float cap[1..N] = ...;

dvar float+ r;
dvar boolean z[1..N];
dvar boolean y[1..N][1..N][1..N][1..N];

minimize r;

subject to{
	sum(k in 1..N) z[k] == p;
	forall(i in 1..N, j in 1..N) sum(k in 1..N, l in 1..N) y[i][j][k][l] == 1;
	forall(i in 1..N, j in 1..N, l in 1..N) sum(k in 1..N) y[i][j][k][l] <= z[l];
	forall(i in 1..N, j in 1..N, k in 1..N) sum(l in 1..N) y[i][j][k][l] <= z[k];
	
	forall(k in 1..N) sum(i in 1..N, j in 1..N) w[i][j] * ((sum(l in 1..N) (y[i][j][k][l] + y[i][j][l][k])-y[i][j][k][k])) <= cap[k];
	
	forall(i in 1..N, j in 1..N) sum(k in 1..N, l in 1..N) (betha*c[i][k] + alpha*c[k][l] + gama*c[l][j]) * y[i][j][k][l] <= r;
	
}
 