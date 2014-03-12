/*
 * main.cpp
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */

#include "common.h"
#include "RegisteredModularityBInstance.hpp"
#include "BranchAndBound.hpp"

#include "Timer.hpp"

#include "VnsLabelPropagation.h"
#include "Divisive.h"
//std::string  RegisteredModularityBInstance::InstancesPath = "C:\\Users\\manuel\\Documents\\Github\\clusterisator\\bipartite_instances\\";

std::string RegisteredModularityBInstance::InstancesPath = "../txt/";

int usage() {
	std::cout << "Available instances : \n";
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
	std::cout << "<exe> <id of selected instance> <number of labels>\n";
	std::cout << "The program launch the multi level algorithm\n";
	return 0;
}

int main(int argc, char** argv) {
	Timer total;
	if (argc == 1)
		return usage();

	AvailableModularityBInstances id(
			static_cast<AvailableModularityBInstances>(atoi(argv[1]) - 1));
	bool doStabilization(false);
	if (argc > 2)
		doStabilization = atoi(argv[2]) != 0;
	//return vns(argc, argv);
	//RegisteredModularityBInstance instance(south);
	//RegisteredModularityBInstance instance(SupremeCourtyes);
	//RegisteredModularityBInstance instance(SupremeCourtnot);
	//RegisteredModularityBInstance instance(SocialWorkJ);
	RegisteredModularityBInstance instance(id);

	size_t id_node(0);
	std::string file_name;
	do {
		file_name = GetStr("node/", instance.name(), "_node_", id_node);
		++id_node;
	} while (std::remove(file_name.c_str()) == 0);
	//RegisteredModularityBInstance instance(divorces);
	//RegisteredModularityBInstance instance(hollyw);
	//RegisteredModularityBInstance instance(scotl);
	//RegisteredModularityBInstance instance(graphprod);
	//RegisteredModularityBInstance instance(netscience);
	instance.out();
	std::cout << instance.a() << std::endl;

	IndexedList allNodes(instance.nV(), true);
	ModularityBPartition p(instance, instance.nV());

	p.score() = p.computeScore();
	std::cout << "DIVISIVE STARTED" << std::endl;
	Divisive divisive(instance, p);
	divisive.run();
	std::cout << "VNS STARTED" << std::endl;
	VnsLabelPropagation vns(instance, p, 2, 5);
	vns.run();
	std::cout << "B&B STARTED" << std::endl;

	BranchAndBound branchAndBound(instance);
	branchAndBound.init();

	branchAndBound.master().add(p);
	//branchAndBound.master().write();
	if (doStabilization)
		branchAndBound.master().buildStabilization(p);
	//branchAndBound.master().write();
//	exit(0);
	branchAndBound.run();
//	branchAndBound.writeSolution();
	std::cout << "program run in " << std::setprecision(10) << total.elapsed()
			<< std::endl;
	return 0;
}

