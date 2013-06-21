/*
* main.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/

#include "common.h"
#include "Partition.hpp"
#include "Number.hpp"
#include <random>
#include <iostream>
#include <fstream>
#include "RegisteredModularityInstance.hpp"
#include "BipartiteGraph.hpp"

int main(int argc, char** argv){
	BipartiteGraph g("D:\\GitHub\\clusterisator\\bipartite_instances\\divorces.dat");
	g.exportAmpl("D:\\GitHub\\clusterisator\\bipartite_instances\\divorces2.dat");
	//RegisteredModularityInstance::InstancesPath = "C:/Users/manuel/Documents/GitHub/clusterisator-master/modularity_instances/";
	//for(size_t i(0); i<AvailableModularityInstancesSize; ++i){
	//	AvailableModularityInstances id(static_cast<AvailableModularityInstances>(i));
	//	RegisteredModularityInstance instance(id);
	//	instance.exportAmpl(RegisteredModularityInstance::InstancesPath +instance.name+".dat");
	//}
	return 0;
}