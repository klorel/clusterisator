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

class ColmunHandler {
public:

};

void columns_generation(RegisteredModularityBInstance & instance, ampl::AMPL & ampl) {
	int cg_stop = 0;
	int cg_ite = 0;
	double cg_cols = 0;
	double cg_bound = 0;
	double cg_reduced_cost = 0;
	double cg_added = 0;
	double cg_success = 0;


	DoubleVector dual;
	DecisionList decisions;

	VnsGenerator vnsOracle(&instance);
	instance.setVnsOracle(&vnsOracle);
	vnsOracle.setData(dual, decisions);

	ampl::Set V = ampl.getSet("V");
	ampl::Parameter DUAL_FOR_RC = ampl.getParameter("DUAL_FOR_RC");
	dual.assign(V.size(), 0);

	ampl.getParameter("CHECK_RC").set(1);
	while (cg_stop != 1) {
		++cg_ite;
		ampl.read("master.run");

		//for (auto & t : V.members()) {
		//	std::cout << t.toString() << " = " << DUAL_FOR_RC.get(t).toString() << std::endl;
		//}

		ampl.read("slave_exact.run");
		cg_added = ampl.getValue("CG_ADDED").dbl();
		cg_success = ampl.getValue("CG_SUCCESS").dbl();

		if (cg_success > 0) {
			ampl.read("add_column.run");
		}
		else {
			cg_stop = 1;
		};

		cg_cols = ampl.getValue("CG_COLS").dbl();
		cg_bound = ampl.getValue("CG_BOUND").dbl();
		cg_reduced_cost = ampl.getValue("CG_REDUCED_COST").dbl();

		std::cout << std::setw(6) << cg_ite;
		std::cout << std::setw(6) << cg_cols;
		std::cout << std::setw(6) << cg_added;
		std::cout << std::setw(15) << std::setprecision(8) << cg_bound;
		std::cout << std::setw(15) << std::setprecision(8) << cg_reduced_cost;
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
