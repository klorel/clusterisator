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
	
	ampl::Parameter DUAL_FOR_RC = ampl.getParameter("DUAL_FOR_RC");
	int i(0);
	for (auto & t : DUAL_FOR_RC) {
		double const d = t.second.dbl();
		dual[i] = -d;
		++i;
	}
	generator.clear();
	generator.vns();
	//std::cout << generator.rc() << std::endl;
	bool success(generator.rc() > ZERO_REDUCED_COST);
	if (success) {
		generator.addNeighbor(1);
		int n = 0;
		ampl::Set pool_cost = ampl.getSet("POOL_COST");
		ampl::Set pool_solution = ampl.getSet("POOL_SOLUTION");
		std::vector<ampl::Tuple> pool_solution_tuples;
		std::vector<ampl::Tuple> pool_cost_tuples;
		for (auto const & kvp : generator.result()) {
			n += 1;
			
			ampl::Tuple t_cost({ n, kvp.second->cost(), kvp.second->reducedCost() });
			pool_cost_tuples.push_back(t_cost);
			bool is_first = true;
			for (auto const i : kvp.second->v()) {
				ampl::Tuple t_solution({ n, i_to_v[i] });
				pool_solution_tuples.push_back(t_solution);
			}
		}
		pool_solution.setValues(pool_solution_tuples.data(), (int)pool_solution_tuples.size());
		pool_cost.setValues(pool_cost_tuples.data(), (int)pool_cost_tuples.size());
	}
	return success;
}

void get_heuristic(ampl::AMPL & ampl, AmplColumns const & columns) {

}

void get_exact(ampl::AMPL & ampl, AmplColumns const & columns) {
	ampl.read("slave_exact.run");
}

void add_column(ampl::AMPL & ampl, AmplColumns const & columns) {

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
	ampl::Parameter cg_reduced_cost = ampl.getParameter("CG_REDUCED_COST");

	DoubleVector dual;
	DecisionList decisions;

	VnsGenerator vnsOracle(&instance);
	instance.setVnsOracle(&vnsOracle);
	vnsOracle.setData(dual, decisions);

	ColumnGenerator generator(&instance);
	generator.setVns(&vnsOracle, dual, decisions);
	generator.setNumberByIte(0);
	ampl::Set V = ampl.getSet("V");
	ampl::Parameter DUAL_FOR_RC = ampl.getParameter("DUAL_FOR_RC");
	dual.assign(V.size(), 0);
	int max_v(0);
	for (auto & t : DUAL_FOR_RC) {
		int v = static_cast<int>(t.first[0].dbl());
		max_v = std::max(max_v, v);
	}
	std::vector<int> v_to_i(max_v + 1);
	std::vector<int> i_to_v(dual.size());
	int i(0);
	for (auto & t : DUAL_FOR_RC) {
		int v = static_cast<int>(t.first[0].dbl());
		v_to_i[v-1] = i;
		i_to_v[i] = v;
		//std::cout << std::setw(6) << i;
		//std::cout << std::setw(6) << v;
		//std::cout << std::endl;
		++i;
	}

	AmplColumns all_columns;
	AmplColumns pool;

	extract_pool(ampl, "ALL_COST", "ALL_SOLUTION", all_columns);
	std::string step;
	while (cg_stop.get().dbl() != 1.0) {
		++cg_ite;
		ampl.read("master.run");
		step = "UNKNOWN";

		launch_heuristic(ampl, dual, generator, i_to_v);
		
		if (cg_success.get().dbl() == 0) {
			ampl.read("slave_exact.run");
			step = "EXACT";
		}
		else {
			step = "VNS";
		}
		//extract_pool(ampl, "POOL_COST", "POOL_SOLUTION", pool);
		
		ampl.read("check_and_add.run");
		
		if (cg_stop.get().dbl() == 0) {
			int n = merge_pool(pool, all_columns);
			if (n > 0) {
				std::cout << "column already there" << std::endl;
			}
		}


		std::cout << std::setw(6) << cg_ite;
		std::cout << std::setw(6) << cg_cols.get().dbl();
		std::cout << std::setw(6) << cg_added.get().dbl();
		std::cout << std::setw(6) << step;
		std::cout << std::setw(15) << std::setprecision(8) << cg_bound.get().dbl();
		std::cout << std::setw(15) << std::setprecision(8) << cg_reduced_cost.get().dbl();
		std::cout << std::endl;
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
