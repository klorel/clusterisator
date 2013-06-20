/*
* main.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/

#include "src/common.h"
#include "src/Partition.hpp"

#include "src/KMAlgo.hpp"
#include "src/KMInstance.hpp"
#include "src/KMConstraints.hpp"
#include "src/KMPartition.hpp"

#include "src/Number.hpp"
#include "src/RegisteredInstance.hpp"

#include <random>
#include <iostream>
#include <fstream>
#include "../gencol/RegisteredModularityInstance.hpp"


int main(int argc, char** argv){
	RegisteredModularityInstance::InstancesPath = "C:/Users/manuel/Documents/GitHub/clusterisator-master/modularity_instances/";
	for(size_t i(0); i<AvailableModularityInstancesSize; ++i){
		AvailableModularityInstances id(static_cast<AvailableModularityInstances>(i));
		RegisteredModularityInstance instance(id);
		instance.exportAmpl(RegisteredModularityInstance::InstancesPath +instance.name+".dat");
	}
	return 0;
}