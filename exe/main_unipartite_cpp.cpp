/*
 * main.cpp
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */


#include "../unipartite_data/RegisteredModularityInstance.h"

#include "../columnsgeneration/CliquePartitionProblem.h"

#include "../clustering/Timer.h"
#include "../clustering/common.h"


#include "../mip_solver/XpressSolver.h"
#include "../mip_solver/CplexSolver.h"

std::string RegisteredModularityInstance::InstancesPath = "../txt/";

int usage() {
  std::cout << "Available instances : " << std::endl;
  for (int i(0);
      i < AvailableModularityInstances::AvailableModularityInstancesSize; ++i) {
    AvailableModularityInstances id(
        static_cast<AvailableModularityInstances>(i));
    RegisteredModularityInstance instance(id);
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

	AvailableModularityInstances id(static_cast<AvailableModularityInstances>(atoi(argv[1]) - 1));
	RegisteredModularityInstance instance(id);

	//	int id_node(0);
	//	std::string file_name;
	//	do {
	//		file_name = GetStr("node/", instance.name(), "_node_", id_node);
	//		++id_node;
	//	} while (std::remove(file_name.c_str()) == 0);

	instance.out();
	CliquePartitionProblem cpp;
	instance.getCliquePartitionProblem(cpp);
	//XpressSolver xpress;
	//
	//xpress.stream().push_back(&std::cout);
	//xpress.initLp("cps_xpress");
	//xpress.setNbThreads(4);
	//
	//xpress.setLog();
	//cpp.cps("cps_xpress", xpress);

	CplexSolver cplex;
	cplex.initLp("cps_cplex");
//	cplex.setNbThreads(4);
	cplex.setLog();
	cplex.add(std::cout);
	cpp.cps("cps_cplex", cplex);
	return 0;
}

//
//int main(int argc, char** argv) {
//  Timer total;
//  if (argc == 1)
//    return usage();
//  std::cout << "argc is " << argc << std::endl;
//  AvailableModularityInstances id(
//      static_cast<AvailableModularityInstances>(atoi(argv[1]) - 1));
//
//  AvailableOracle oracle(AvailableOracle::MILP);
//  if (argc > 2) {
//    oracle = static_cast<AvailableOracle>(atoi(argv[2]));
//  }
//
//  RegisteredModularityInstance instance(id);
//  int id_node(0);
//  std::string file_name;
//  do {
//    file_name = GetStr("node/", instance.name(), "_node_", id_node);
//    ++id_node;
//  } while (std::remove(file_name.c_str()) == 0);
//
//  instance.out();
//  //	std::cout << instance.a() << std::endl;
//
//  //	ModularityBPartition p(instance, instance.nV());
//  //	p.score() = p.computeScore();
//  //	std::cout << "DIVISIVE STARTED" << std::endl;
//  //	Divisive divisive(instance, p);
//  //	divisive.run();
//  //	std::cout << "VNS STARTED" << std::endl;
//  //	VnsLabelPropagation vns(instance, p, 20, 5);
//  //	vns.run();
//  //	std::cout << "B&B STARTED" << std::endl;
//
//  //	return 0;
//
//  // oracles creation
//  VnsGenerator vnsOracle(&instance);
//  instance.setVnsOracle(&vnsOracle);
//
//  CliquePartitionProblem cpp;
//  instance.getCliquePartitionProblem(cpp);
//
//  MilpOracle milpOracle(&cpp);
//  QpOracle miqpOracle(&cpp);
//  switch (oracle) {
//    case MIQP:
//      instance.setExactOracle(&miqpOracle);
//      break;
//    case MILP:
//    default:
//      instance.setExactOracle(&milpOracle);
//      break;
//  }
//  BranchAndBound branchAndBound(instance);
//  branchAndBound.init();
//  //	branchAndBound.master().add(p);
//  //	branchAndBound.master().addEdge();
//  branchAndBound.master().addSingleton();
//  //	branchAndBound.master().write();
//  branchAndBound.setOutput();
//  branchAndBound.run();
//  //	for (int i(0); i < 1; ++i)
//  //		branchAndBound._vnsGenerator->run(1, false);
//  //	branchAndBound.writeSolution();
//  std::cout << "program run in " << std::setprecision(10) << total.elapsed()
//            << std::endl;
//  std::cout << "Solution is " << branchAndBound.bestFeasible() + instance.cst()
//            << std::endl;
////	instance.cps(instance.name());
//  return 0;
//}

