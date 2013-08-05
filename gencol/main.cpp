/*
* main.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/

#include "common.h"
#include "RegisteredModularityBInstance.hpp"
#include "BranchAndBound.hpp"


std::string  RegisteredModularityBInstance::InstancesPath = "../instances/";

int main(int argc, char** argv){
	//return vns(argc, argv);
	RegisteredModularityBInstance::InstancesPath = "C:\\Users\\manuel\\Documents\\Github\\clusterisator\\bipartite_instances\\";
	RegisteredModularityBInstance instance(south);
	//RegisteredModularityBInstance instance(women);
	//RegisteredModularityBInstance instance(divorces);
	instance.out();
	//ModularityBPartition p(instance, instance.nbNodes());
	BranchAndBound branchAndBound(instance);
	//master.readColumns("columns.txt");
	//master.readColumns("bug.txt");
	//master.write();
	//master.writeOracle();
	branchAndBound.columnGeneration();
	return 0;
}