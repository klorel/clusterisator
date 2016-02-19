/*
 * main.cpp
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */

#include "common.h"
#include "RegisteredModularityInstance.hpp"

#include "BranchAndBound.hpp"

#include "Timer.hpp"

#include "VnsLabelPropagation.h"
#include "Divisive.h"

std::string RegisteredModularityInstance::InstancesPath = "../txt/";

int usage() {
	std::cout << "Available instances : \n";
	for (size_t i(0);
			i < AvailableModularityInstances::AvailableModularityInstancesSize;
			++i) {
		AvailableModularityInstances id(
				static_cast<AvailableModularityInstances>(i));
		RegisteredModularityInstance instance(id);
		std::cout << std::setw(3) << i + 1;
		std::cout << " : ";
		std::cout << std::setw(30) << std::left << instance._name << std::right;

		std::cout << "\n";
	}
	std::cout
			<< "<exe> <id of selected instance> <oracle in 0(MIQP),1(MILP)2(bMILP default)>\n";
	std::cout << "The program launch the column generation algorithm\n";
	return 0;
}

int main(int argc, char** argv) {
	return 0;
}

