/*
 * main.cpp
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */

#include "../bipartite_data/RegisteredModularityBInstance.h"
#include "../columnsgeneration/CliquePartitionProblem.h"

#include "../clustering/Timer.h"
#include "../clustering/common.h"

#include "../mip_solver/XpressSolver.h"
#include "../mip_solver/CplexSolver.h"

std::string RegisteredModularityBInstance::InstancesPath = "../txt/";

int usage() {
  std::cout << "Available instances : " << std::endl;
  for (int i(0);
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
      << "<exe> <id of selected instance> <oracle in 0(MIQP),1(MILP default)>"
      << std::endl;
  std::cout << "The program launch the column generation algorithm"
            << std::endl;
  return 0;
}

int main(int argc, char** argv) {
  Timer total;
  if (argc == 1)
    return usage();

  AvailableModularityBInstances id(static_cast<AvailableModularityBInstances>(atoi(argv[1]) - 1));
  RegisteredModularityBInstance instance(id);

//	int id_node(0);
//	std::string file_name;
//	do {
//		file_name = GetStr("node/", instance.name(), "_node_", id_node);
//		++id_node;
//	} while (std::remove(file_name.c_str()) == 0);

  instance.out();
  CliquePartitionProblem cpp;
  instance.getCliquePartitionProblem(cpp);
  XpressSolver xpress;
  //cpp.cps("cps_xpress", xpress);
  CplexSolver cplex;
  cplex.initLp("cps_cplex");
  cplex.setNbThreads(4);
  cplex.setLog();
  cplex.add(std::cout);
  cpp.cps("cps_cplex", cplex);
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
