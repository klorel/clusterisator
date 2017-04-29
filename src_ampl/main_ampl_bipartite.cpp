#include <ampl/ampl.h>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "../bipartite_data/RegisteredModularityBInstance.h"

#include "../clustering/Timer.h"
#include "../clustering/common.h"


#include "../columnsgeneration/BranchAndBound.h"
#include "../columnsgeneration/VnsGenerator.h"

std::string RegisteredModularityBInstance::InstancesPath = "../txt/";

int usage() {
	std::cout << "Available instances : " << std::endl;
	for (int i(0); i < AvailableModularityBInstances::AvailableModularityBInstancesSize; ++i) {
		AvailableModularityBInstances id(static_cast<AvailableModularityBInstances>(i));
		RegisteredModularityBInstance instance(id);
		std::cout << std::setw(3) << i + 1;
		std::cout << " : ";
		std::cout << std::setw(30) << std::left << instance._name << std::right;
		std::cout << "\n";
	}
	std::cout << "<exe> <id of selected instance>" << std::endl;
	std::cout << "The program launch the column generation algorithm" << std::endl;
	return 0;
}
/*
* Class used as an output handler. It only prints the solver output.
* Must publicly implement ``ampl::OutputHandler``.
*/
class MyOutputHandler : public ampl::OutputHandler {
public:
	void output(ampl::output::Kind kind, const char* output) {
		//if (kind == ampl::output::SOLVE)
		//	std::printf("Solver: %s\n", output);
		std::cout << output;
	}
};
class AmplColumn;
class AmplColumnPredicate;
typedef std::shared_ptr<AmplColumn> AmplColumnPtr;
typedef std::set<AmplColumnPtr, AmplColumnPredicate> AmplColumns;

class AmplColumn {
public:
	std::set<int> & v() { return _v; }
	double &rc() { return _rc; }
	double &cost() { return _cost; }

	std::set<int> const & v() const { return _v; }
	double rc() const { return _rc; }
	double cost() const { return _cost; }
private:
	std::set<int> _v;
	double _rc;
	double _cost;
};
class AmplColumnPredicate {
public:
	bool operator()(AmplColumn const & c1, AmplColumn const & c2) {
		return c1.v() < c2.v();
	}
	bool operator()(AmplColumnPtr const & p1, AmplColumnPtr const & p2) {
		return p1->v() < p2->v();
	}
};



class ColmunHandler {
public:
	AmplColumns _columns;
public:
};
void print(AmplColumnPtr const & rhs, std::ostream & stream) {
	for (auto const & v : rhs->v()) {
		stream << v << " ";
	}
	stream << std::endl;

}
void extract_pool(ampl::AMPL & ampl, std::string const & name_cost, std::string const & name_solution, AmplColumns & result) {
	result.clear();
	std::map<int, AmplColumnPtr> temp;
	ampl::Set pool_cost = ampl.getSet(name_cost);
	for (auto const & t : pool_cost.getValues()) {
		int i = static_cast<int>(t[0].dbl());
		AmplColumnPtr ptr(new AmplColumn);
		ptr->cost() = t[1].dbl();
		ptr->rc() = t[2].dbl();
		temp[i] = ptr;
		//std::cout << std::setw(8) << t[0].toString();
		//std::cout << std::setw(8) << t[1].toString();
		//std::cout << std::setw(8) << t[2].toString();
		//std::cout << std::endl;
	}
	ampl::Set pool_solution = ampl.getSet(name_solution);
	for (auto const & t : pool_solution.getValues()) {
		int i = static_cast<int>(t[0].dbl());
		int v = static_cast<int>(t[1].dbl());
		//std::cout << std::setw(8) << t[0].toString();
		//std::cout << std::setw(8) << t[1].toString();
		//std::cout << std::endl;
		if (!temp[i]->v().insert(v).second) {
			std::cout << "DUPLICATE ELEMENT IN AMPL COLUMN" << std::endl;
		}
	}
	for (auto const & kvp : temp) {
		if (!result.insert(kvp.second).second) {
			std::cout << "DUPLICATE COLUMN IN AMPL POOL" << std::endl;
			print(kvp.second, std::cout);
		}
	}
	//for (auto const & column : result) {
	//	print(column, std::cout);
	//}
}


bool launch_heuristic(ampl::AMPL & ampl, DoubleVector & dual, ColumnGenerator & generator, std::vector<int> const & i_to_v) {
	bool success(false);
	ampl::Parameter PI = ampl.getParameter("PI");
	ampl::Constraint convexity = ampl.getConstraint("convexity");
	int i(0);
	for (auto & t : PI) {
		double const d = t.second.dbl();
		if (i >= dual.size()) {
			std::cout << i << " upper dual size " << dual.size() << std::endl;
			std::exit(0);
		}
		dual[i] = -d;
		++i;
	}
	generator.clear();
	//std::cout << "vns ... "<<std::endl;
	generator.vns();
	//std::cout << "... end" << std::endl;
	
	//std::cout << generator.rc() << std::endl;
	success = generator.rc() > ZERO_REDUCED_COST * 10;
	if (success) {
		generator.addNeighbor(1);
		int n = 0;
		ampl::Set pool_cost = ampl.getSet("POOL_COST");
		ampl::Set pool_solution = ampl.getSet("POOL_SOLUTION");
		std::vector<ampl::Tuple> pool_solution_tuples;
		std::vector<ampl::Tuple> pool_cost_tuples;
		for (auto const & kvp : generator.result()) {
			if (kvp.first > ZERO_REDUCED_COST*10) {
				//std::cout << "n : "<<n << std::endl;
				n += 1;
				ampl::Tuple t_cost({ n, kvp.second->cost(), kvp.second->reducedCost() });
				pool_cost_tuples.push_back(t_cost);
				bool is_first = true;
				//std::cout << n << " : ";
				//for (auto const i : kvp.second->v()) {
				//	std::cout << i << " ";
				//}
				//std::cout << std::endl;
				for (auto const i : kvp.second->v()) {
					//std::cout << std::setw(6) << n;
					//std::cout << std::setw(6) << i;
					//std::cout << std::setw(6) << i_to_v[i];
					//std::cout << std::endl;
					ampl::Tuple t_solution({ n, i_to_v[i]});
					pool_solution_tuples.push_back(t_solution);
				}
			}
		}
		pool_solution.setValues(pool_solution_tuples.data(), (int)pool_solution_tuples.size());
		pool_cost.setValues(pool_cost_tuples.data(), (int)pool_cost_tuples.size());
		ampl.getParameter("STEP").set("VNS");
	}
	return success;
}

void get_heuristic(ampl::AMPL & ampl, AmplColumns const & columns) {

}

void get_exact(ampl::AMPL & ampl, AmplColumns const & columns) {
	ampl.read("slave_exact.run");
}

void add_column(ampl::AMPL & ampl) {
	ampl::Set pool_solution = ampl.getSet("POOL_SOLUTION");
	ampl::Set pool_cost = ampl.getSet("POOL_COST");
	ampl::Parameter cg_added = ampl.getParameter("CG_ADDED");
	ampl::Set all_columns = ampl.getSet("ALL_COLUMNS");
	ampl::Set all_cost = ampl.getSet("ALL_COST");
	ampl::Set cols = ampl.getSet("COLS");
	//cols.getValues().
	int current_size(static_cast<int>(all_cost.size()));
	std::cout << "all_cost.size()      : " << all_cost.size() << std::endl;
	std::cout << "all_columns.size()   : " << all_columns.size() << std::endl;
	std::cout << "pool_cost.size()     : " << pool_cost.size() << std::endl;
	std::cout << "pool_solution.size() : " << pool_solution.size() << std::endl;
	std::vector<ampl::Tuple> new_all_cost(all_cost.size() + pool_cost.size());
	int i(0); 
	for (auto t : all_cost.getValues()) {
		new_all_cost[i] = { t[0], t[1], t[2] };
		++i;
	}
	int n(0);
	std::vector<double> rc(pool_cost.size(), 0);
	for (auto const & kvp : pool_cost.getValues()) {
		rc[(int)(kvp[0].dbl()-1)] = kvp[2].dbl();
		if (kvp[2].dbl() > ZERO_REDUCED_COST) {
			new_all_cost[i] = { current_size+kvp[0].dbl(), kvp[1], kvp[2] };
			++i;
			++n;
		}
	}
	std::cout << "n = " << n << std::endl;
	std::vector<ampl::Tuple> new_all_solution(all_columns.size() + pool_solution.size());
	i = 0;
	for (auto t : all_columns.getValues()) {
		new_all_solution[i] = { t[0], t[1] };
		++i;
	}
	for (auto const & kvp : pool_solution.getValues()) {
		if (rc[(int)(kvp[0].dbl()-1)] > ZERO_REDUCED_COST) {
			new_all_solution[i] = { current_size+kvp[0].dbl(), kvp[1]};
			++i;
		}
	}
//	ampl.getSet("TOTO_COST").setValues(new_all_cost.data(), n);
//	ampl.getSet("TOTO_COLUMNS").setValues(new_all_solution.data(), i);

}
int merge_pool(AmplColumns const & lhs, AmplColumns & rhs) {
	int already_there(0);
	for (auto const & ptr : lhs) {
		if (!rhs.insert(ptr).second) {
			++already_there;
		}
	}
	return already_there;
}
void columns_generation(RegisteredModularityBInstance & instance, ampl::AMPL & ampl) {
	int cg_ite = 0;

	ampl::Parameter cg_stop = ampl.getParameter("CG_STOP");
	ampl::Parameter cg_success = ampl.getParameter("CG_SUCCESS");
	ampl::Parameter cg_added = ampl.getParameter("CG_ADDED");

	ampl::Parameter cg_cols = ampl.getParameter("CG_COLS");
	ampl::Parameter cg_bound = ampl.getParameter("CG_BOUND");
	ampl::Objective master_obj = ampl.getObjective("MASTER_OBJ");
	ampl::Parameter cg_reduced_cost = ampl.getParameter("CG_REDUCED_COST");

	ampl::Parameter use_stab = ampl.getParameter("USE_STAB");
	ampl::Parameter bundle_step = ampl.getParameter("BUNDLE_STEP");
	ampl::Parameter feas_error = ampl.getParameter("FEAS_ERROR");
	ampl::Parameter opt_error = ampl.getParameter("OPT_ERROR");
	ampl::Parameter phi_pi = ampl.getParameter("PHI_PI");
	ampl::Parameter phi_pi_bar = ampl.getParameter("PHI_PI_BAR");
	ampl::Parameter c_dot_x = ampl.getParameter("C_DOT_X");

	DoubleVector dual;
	DecisionList decisions;

	VnsGenerator vnsOracle(&instance);
	instance.setVnsOracle(&vnsOracle);
	vnsOracle.setData(dual, decisions);

	ColumnGenerator generator(&instance);
	generator.setVns(&vnsOracle, dual, decisions);
	generator.setNumberByIte(1);
	ampl::Set V = ampl.getSet("V");
	ampl::Parameter PI = ampl.getParameter("PI");
	dual.assign(instance.nV(), 0);
	std::vector<int> i_to_v(dual.size(), -1);
	for (int i(0); i < i_to_v.size(); ++i) {
		i_to_v[i] = i + 1;
	}

	int max_v(0);
	for (auto & t : PI) {
		int v = static_cast<int>(t.first[0].dbl());
		max_v = std::max(max_v, v);
		//std::cout << t.first.toString() << " - ";
		//std::cout << std::setw(6) << v;
		//std::cout << std::endl;
	}
	std::vector<int> v_to_i(max_v + 1, -1);
	int i(0);
	std::cout << "max_v is " << max_v << std::endl;
	for (auto & t : PI) {
		int v = static_cast<int>(t.first[0].dbl());
		v_to_i[v] = i;
		//std::cout << std::setw(6) << i;
		//std::cout << std::setw(6) << v;
		//std::cout << std::endl;
		++i;
	}
	//for (int v(0); v < v_to_i.size(); ++v) {
	//	std::cout << std::setw(6) << v;
	//	std::cout << std::setw(6) << v_to_i[v];
	//	std::cout << std::endl;
	//}
	//std::exit(0);
	AmplColumns all_columns;
	AmplColumns pool;

	extract_pool(ampl, "ALL_COST", "ALL_COLUMNS", all_columns);
	std::string step;
	std::cout << "begin columns generation" << std::endl;
	int counter(0);
	while (cg_stop.get().dbl() != 1.0) {
		++cg_ite;
		ampl.read("master.run");
		ampl.eval("problem DEFAULT;");
		step = "??";

		launch_heuristic(ampl, dual, generator, i_to_v);
		++counter;
		//if (counter >= 5 || cg_success.get().dbl() == 0) {
		//	ampl.read("slave_exact.run");
		//	counter = 0;
		//}
//		if (cg_success.get().dbl() == 0) {
////			ampl.read("slave_exact.run");
////			ampl.eval("problem DEFAULT;");
//			step = "EXACT";
//		}
//		else {
//			step = "VNS";
//		}
//		//extract_pool(ampl, "POOL_COST", "POOL_SOLUTION", pool);
		if (cg_success.get().dbl() != 0) {
			ampl.read("add_column.run");
//			add_column(ampl);
		}
		ampl.read("check_and_add.run");
		ampl.eval("problem DEFAULT;");
		
		if (cg_stop.get().dbl() == 0) {
			int n = merge_pool(pool, all_columns);
			if (n > 0) {
				std::cout << "column already there" << std::endl;
			}
		}

		if (use_stab.get().dbl() == 0) {
			std::cout << std::setw(6) << cg_ite;
			std::cout << std::setw(6) << cg_cols.get().dbl();
			std::cout << std::setw(6) << cg_added.get().dbl();
			std::cout << std::setw(6) << step;
			//std::cout << std::setw(17) << std::setprecision(10) << std::scientific << cg_bound.get().dbl();
			std::cout << std::setw(17) << std::setprecision(10) << std::scientific << master_obj.get().value();
			std::cout << std::setw(17) << std::setprecision(10) << std::scientific << cg_reduced_cost.get().dbl();
			std::cout << std::defaultfloat << std::endl;
		}
		else {
			std::cout << std::setw(6) << cg_ite;
			std::cout << std::setw(6) << cg_cols.get().dbl();
			std::cout << std::setw(6) << cg_added.get().dbl();
			std::cout << std::setw(6) << step;
			//std::cout << std::setw(17) << std::setprecision(10) << std::scientific << cg_bound.get().dbl();
			std::cout << std::setw(17) << std::setprecision(10) << std::scientific << master_obj.get().value();

			std::cout << std::setw(17) << std::setprecision(10) << std::scientific << cg_reduced_cost.get().dbl();

			std::cout << std::setw(10) << bundle_step.get().str();
			std::cout << std::setw(17) << std::setprecision(10) << std::scientific << feas_error.get().dbl();
			std::cout << std::setw(17) << std::setprecision(10) << std::scientific << opt_error.get().dbl();
			if (bundle_step.get().str() != "XS") {
				std::cout << std::setw(17) << std::setprecision(10) << std::scientific << phi_pi.get().dbl();
				std::cout << std::setw(17) << std::setprecision(10) << std::scientific << (phi_pi_bar.get().dbl() > 1e10 ? -1 : phi_pi_bar.get().dbl());
			} else {
				std::cout << std::setw(17) << "-";
				std::cout << std::setw(17) << "-";
			};
			std::cout << std::setw(17) << std::setprecision(10) << std::scientific << c_dot_x.get().dbl();
			std::cout << std::defaultfloat << std::endl;

		}
	}
}

int main(int argc, char **argv) {


	Timer total;
	if (argc == 1)
		return usage();

	AvailableModularityBInstances id(static_cast<AvailableModularityBInstances>(atoi(argv[1]) - 1));
	
	RegisteredModularityBInstance instance(id);

	instance.out();
	// oracles creation
	std::cout << "nV() : "<<instance.nV() << std::endl;

	try {
		ampl::AMPL ampl;
		MyOutputHandler outputHandler;
		ampl.setOutputHandler(&outputHandler);
		ampl.read("main.run");

		std::stringstream buffer;
		buffer << "data; set E := include ../txt/" << instance.name() << ".txt;model;";
		std::cout << buffer.str() << std::endl;
		ampl.eval(buffer.str().c_str());

		ampl.read("init.run");
		//ampl.read("columns_generation.run");
		columns_generation(instance, ampl);

	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << "\n";
	}
}
