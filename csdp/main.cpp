#include "CsdpInterface.h"
//#include "ModularityPartition.hpp"
#include "RegisteredModularityInstance.hpp"

//int usage() {
//	std::cout << "Available instances : \n";
//	for (size_t i(0);
//			i < AvailableModularityInstances::AvailableModularityInstancesSize;
//			++i) {
//		AvailableModularityInstances id(
//				static_cast<AvailableModularityInstances>(i));
//		RegisteredModularityInstance instance(id);
//		std::cout << std::setw(3) << i + 1;
//		std::cout << " : ";
//		std::cout << std::setw(30) << std::left << instance.name << std::right;
//
//		std::cout << "\n";
//	}
//	std::cout << "<exe> <id of selected instance> \n";
//	return 0;
//}

void example(CsdpInterface & csdpInterface) {
	csdpInterface.clear();
	csdpInterface.add(1, 1, -1);
	csdpInterface.add(2, 2, +1);
	csdpInterface.add(3, 3, 3);
	csdpInterface.add(4, 4, 2);
	csdpInterface.add(5, 5, 3);
	csdpInterface.add(6, 6, 0);
	csdpInterface.add(7, 7, 0);
}

int main(int argc, char**argv) {
//	CsdpInterface csdpInterface;
//	example(csdpInterface);
//	std::map<std::size_t, double> result;
//	csdpInterface.bestReformulation(result);
//	RegisteredModularityInstance instance(AvailableModularityInstances::karate);
//	ModularityPartition partition(instance, instance.nbNodes());
//	std::cout << "modularity is " << partition.computeScore() << std::endl;

	return 0;
}
