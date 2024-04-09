#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>

ILOSTLBEGIN

int main(int argc, char *argv[]){
	
	int N, P;
	float alpha, betha, gamma;
	string instancia = "./instancias/";
	instancia += argv[1];

	cout<<instancia;	

	ifstream arq(instancia.c_str());
	if (!arq.is_open()){
		cout << "Error openning file: " << argv[1] << endl;
		arq.close();
		exit(EXIT_FAILURE);
	}

	arq >> N;
	float x[N], y[N];
	float dist[N][N];
	float w[N][N];
	float cap[N];

	for(int i=0;i<N;i++){
		arq >> fixed >> setprecision(6) >> x[i]; arq >> fixed >> setprecision(6) >> y[i]; cout << x[i] << "\t" << y[i] <<endl;
	}

	for(int i=0;i<N;i++){cout<<endl;
		for(int j=0;j<N;j++){
			arq >> fixed >> setprecision(6);
			dist[i][j] = sqrt( pow(x[i]-x[j],2) + pow(y[i]-y[j],2) );cout<<dist[i][j]<<" ";
		}
	}

	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			arq>>w[i][j];

	arq >> P;
	arq >> gamma;
	arq >> alpha;
	arq >> betha;
	
	for(int i=0;i<N;i++)
		arq>>cap[i];

	arq.close();
	
	IloEnv env;

	try{

	IloModel model(env);

	IloNumVar r(env, 0, IloInfinity, ILOFLOAT, "r");
	IloNumVarArray z(env);
	IloArray<IloArray<IloArray<IloBoolVarArray>>> y(env, N);
	
	z.add(IloNumVarArray(env, N, 0, 1, ILOBOOL));
	
	for(int i=0;i<N;i++){
		y[i] = IloArray<IloArray<IloBoolVarArray>>(env, N);
		for(int j=0;j<N;j++){
			y[i][j] = IloArray<IloBoolVarArray>(env, N);
			for(int k=0;k<N;k++)
				y[i][j][k] = IloBoolVarArray(env, N);
		}
	}

	//Função Objetiivo
	model.add(IloMinimize(env, r));

	//Restrições
	
	IloExpr exp1(env);
		for(int k=0;k<N;k++)
			exp1 += z[k];
		model.add(exp1==P);
	exp1.end();

	
	for(int i =0;i<N;i++)
		for(int j=0;j<N;j++){
			IloExpr exp2(env);
			for(int k =0;k<N;k++)
				for(int l=0;l<N;l++)
					exp2 += y[i][j][k][l];
			model.add(exp2==1);
			exp2.end();		
		}
					
	for(int i =0;i<N;i++)
		for(int j=0;j<N;j++)
			for(int l=0;l<N;l++){
				IloExpr exp3(env);
				for(int k=0; k<N; k++)
					exp3 += y[i][j][k][l];
				model.add(exp3 <= z[l]);
				exp3.end();
			}
					
	for(int i =0;i<N;i++)
		for(int j=0;j<N;j++)
			for(int k=0;k<N;k++){
				IloExpr exp4(env);
				for(int l=0; l<N; l++)
					exp4 += y[i][j][k][l];
				model.add(exp4 <= z[k]);
				exp4.end();
			}

	for(int k=0;k<N;k++){
		IloExpr exp6(env);
			for(int i=0;i<N;i++)
				for(int j=0;j<N;j++)
					for(int l=0;l<N;l++)
						exp6 += w[i][j] * y[i][j][k][l];
		model.add(exp6 <= cap[k]);
		exp6.end();
	}
	
	for(int i =0;i<N;i++)
		for(int j=0;j<N;j++){
			IloExpr exp5(env);
			for(int k =0;k<N;k++)
				for(int l=0;l<N;l++)
					exp5 += (gamma*dist[i][k] + alpha*dist[k][l] + betha*dist[l][j]) * y[i][j][k][l];
			model.add(exp5 <= r);
			exp5.end();
		}
		

	
	//Resolvendo modelo
	IloCplex cplex(model);

	cplex.setParam(IloCplex::Param::TimeLimit,7200);
    	//cplex.setParam(IloCplex::Param::MIP::Limits::TreeMemory,1024);
    	//cplex.setParam(IloCplex::Param::Threads,2);
	
	

	//cplex.exportModel("shirt.lp");
	
	cplex.solve();

	env.out() << argv[1] << "\t" << cplex.getStatus() << "\t" << cplex.getBestObjValue() << "\t" << cplex.getObjValue() << "\t" << cplex.getTime();
	
	/*if (cplex.getStatus() == IloAlgorithm::Optimal){
		env.out() << endl << "Custo:" << env.getTime() << endl;
		printf("%.2f",cplex.getObjValue() );
		for(int i=0;i<N;i++)	
			env.out() << cplex.getValue(z[i]) << "\t";
			
			env.out() << endl;						
					 
	} else{
		env.out() << "No Solution" << endl;
	}*/

	ofstream arq("Results_2", ios::out | ios::app );

	arq << argv[1] << "\t" << cplex.getStatus() << "\t" << fixed << setprecision(2) << cplex.getBestObjValue() << "\t" << cplex.getObjValue() << "\t" << cplex.getTime() << endl;
	
	arq.close();

	cplex.end();
	model.end();
	
	} catch (IloException& ex){
		cerr << "IloException: " << ex << endl;
	} catch (...){
		cerr << "Error" << endl;
	}

	env.end();	
	return 0;
}
