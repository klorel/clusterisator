/*
 * main.cpp
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */

#include "../bipartite_data/RegisteredModularityBInstance.h"
#include "../bipartite_data/BinaryDecompositionOracle.h"

#include "../clustering/Timer.h"
#include "../clustering/common.h"

#include "../heuristic/VnsLabelPropagation.h"
#include "../heuristic/Divisive.h"

#include "../columnsgeneration/BranchAndBound.h"
#include "../columnsgeneration/VnsGenerator.h"
#include "../columnsgeneration/MilpOracle.h"
#include "../columnsgeneration/QpOracle.h"

std::string RegisteredModularityBInstance::InstancesPath = "../txt/";

int usage() {
	std::cout << "Available instances : " << std::endl;
	for (size_t i(0);
			i < AvailableModularityBInstances::AvailableModularityBInstancesSize;
			++i) {
		AvailableModularityBInstances id(
				static_cast<AvailableModularityBInstances>(i));
		RegisteredModularityBInstance instance(id);
		std::cout << std::setw(3) << i + 1;
		std::cout << " : ";
		std::cout << std::setw(30) << std::left << instance._name << std::right;

		std::cout << "\n";
	}
	std::cout
			<< "<exe> <id of selected instance> <oracle in 0(MIQP),1(MILP)2(bMILP default)>"
			<< std::endl;
	std::cout << "The program launch the column generation algorithm"
			<< std::endl;
	return 0;
}

int main(int argc, char** argv) {
	Timer total;
	if (argc == 1)
		return usage();

	AvailableModularityBInstances id(
			static_cast<AvailableModularityBInstances>(atoi(argv[1]) - 1));

	AvailableOracle oracle(AvailableOracle::bMILP);
	if (argc > 2) {
		oracle = static_cast<AvailableOracle>(atoi(argv[2]));
	}

	RegisteredModularityBInstance instance(id);

//	size_t id_node(0);
//	std::string file_name;
//	do {
//		file_name = GetStr("node/", instance.name(), "_node_", id_node);
//		++id_node;
//	} while (std::remove(file_name.c_str()) == 0);

	instance.out();

	// oracles creation
	VnsGenerator vnsOracle(&instance);
	instance.setVnsOracle(&vnsOracle);

	MilpOracle milpOracle(&instance);
	QpOracle miqpOracle(&instance);
	BinaryDecompositionOracle bMilpOracle(&instance);
	switch (oracle) {
	case MIQP:
		instance.setExactOracle(&miqpOracle);
		break;
	case MILP:
		instance.setExactOracle(&milpOracle);
		break;
	case bMILP:
	default:
		instance.setExactOracle(&bMilpOracle);
		break;
	}

	BranchAndBound branchAndBound(instance);
	branchAndBound.init();
	branchAndBound.master().addSingleton();
	branchAndBound.setOutput();
	branchAndBound.run();
//	branchAndBound.writeSolution();
	std::cout << "program run in " << std::setprecision(10) << total.elapsed()
			<< std::endl;
	instance.cps("toto");
	return 0;
}

//	std::cout << instance.a() << std::endl;

//	ModularityBPartition p(instance, instance.nV());
//	p.score() = p.computeScore();
//	std::cout << "DIVISIVE STARTED" << std::endl;
//	Divisive divisive(instance, p);
//	divisive.run();
//	std::cout << "VNS STARTED" << std::endl;
//	VnsLabelPropagation vns(instance, p, 20, 5);
//	vns.run();
//	std::cout << "B&B STARTED" << std::endl;
