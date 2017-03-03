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
	std::cout << "Random instance nR nB proba seed" << std::endl;
	return 0;
}

int main(int argc, char** argv) {
	Timer total;
	if (argc == 1)
		return usage();

	Edges edges;
	int nR;
	int nB;
	double proba;
	unsigned int seed;
	{
		std::stringstream buffer(argv[1]);
		buffer >> nR;
	}
	{
		std::stringstream buffer(argv[2]);
		buffer >> nB;
	}
	{
		std::stringstream buffer(argv[3]);
		buffer >> proba;
	}

	{
		std::stringstream buffer(argv[4]);
		buffer >> seed;
	}
	std::srand(seed);
	std::cout << "nR    : " << nR << std::endl;
	std::cout << "nB    : " << nB << std::endl;
	std::cout << "proba : " << proba << std::endl;
	for (int r(0); r < nR; ++r) {
		for (int b(0); b < nB; ++b) {
			int const v = rand() % 100 + 1;
			if (v > proba) {
				edges.push_back(Edge(r, b));
			}
		}
	}
	BipartiteGraph instance(edges);
//	int id_node(0);
//	std::string file_name;
//	do {
//		file_name = GetStr("node/", instance.name(), "_node_", id_node);
//		++id_node;
//	} while (std::remove(file_name.c_str()) == 0);

	// oracles creation
	VnsGenerator vnsOracle(&instance);
	instance.setVnsOracle(&vnsOracle);

	BinaryDecompositionOracle bMilpOracle(&instance);

	instance.setExactOracle(&bMilpOracle);


//	std::cout << "DIVISIVE STARTED" << std::endl;
//	bipartite::Divisive divisive(instance, p);
//	divisive.run();
	//std::cout << "VNS STARTED" << std::endl;
	//bipartite::VnsLabelPropagation vns(instance, p, 20, 5);
	//vns.run();
	std::cout << "B&B STARTED" << std::endl;

	BranchAndBound branchAndBound(instance);

	int nColumnGeneratorNumberByIte(0);
	std::cout << "nColumnGeneratorNumberByIte : " << nColumnGeneratorNumberByIte << std::endl;
	branchAndBound._columnGenerator.setNumberByIte(nColumnGeneratorNumberByIte);

	branchAndBound.init();
	branchAndBound.master().addSingleton();
	branchAndBound.setOutput();
	branchAndBound.run();
//	branchAndBound.writeSolution();
	std::cout << "program run in " << std::setprecision(10) << total.elapsed() << std::endl;
	std::cout << "Solution is " << branchAndBound.bestFeasible() << std::endl;
//	instance.cps("toto");
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
