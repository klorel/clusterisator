/*
* main.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/

#include "common.h"
#include "RegisteredModularityBInstance.hpp"
#include "BranchAndBound.hpp"


//std::string  RegisteredModularityBInstance::InstancesPath = "C:\\Users\\manuel\\Documents\\Github\\clusterisator\\bipartite_instances\\";
std::string  RegisteredModularityBInstance::InstancesPath = "/home/manuel/workspace-cpp/clusterisator/bipartite_instances/";



int usage() {
	std::cout << "Available instances : \n";
	for (size_t i(0); i < AvailableModularityBInstances::AvailableModularityBInstancesSize; ++i) {
		AvailableModularityBInstances id(static_cast<AvailableModularityBInstances>(i));
		RegisteredModularityBInstance instance(id);
		std::cout << std::setw(3) << i+1;
		std::cout << " : ";
		std::cout << std::setw(30) << std::left << instance.name << std::right;

		std::cout << "\n";
	}
	std::cout << "<exe> <id of selected instance> <number of labels>\n";
	std::cout << "The program launch the multi level algorithm\n";
	return 0;
}

int main(int argc, char** argv){
	if(argc==1)
		return usage();
	AvailableModularityBInstances id(static_cast<AvailableModularityBInstances>(atoi(argv[1])-1));
	//return vns(argc, argv);
	//RegisteredModularityBInstance instance(south);
	//RegisteredModularityBInstance instance(SupremeCourtyes);
	//RegisteredModularityBInstance instance(SupremeCourtnot);
	//RegisteredModularityBInstance instance(SocialWorkJ);
	RegisteredModularityBInstance instance(id);
	//RegisteredModularityBInstance instance(divorces);
	//RegisteredModularityBInstance instance(hollyw);
	//RegisteredModularityBInstance instance(scotl);
	//RegisteredModularityBInstance instance(graphprod);
	//RegisteredModularityBInstance instance(netscience);
	instance.out();
	//ModularityBPartition p(instance, instance.nbNodes());
	BranchAndBound branchAndBound(instance);
	//master.readColumns("columns.txt");
	//master.write();
	//master.writeOracle();
	branchAndBound.run();
	return 0;
}
