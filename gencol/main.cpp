/*
 * main.cpp
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */

#include "common.h"
#include "RegisteredModularityBInstance.hpp"
#include "BranchAndBound.hpp"
#include "ModularityBPartition.hpp"
#include "LabelPropagation.hpp"
#include "Timer.hpp"
#include "Divisor.hpp"
std::string  RegisteredModularityBInstance::InstancesPath = "C:\\Users\\manuel\\Documents\\Github\\clusterisator\\bipartite_instances\\";
//std::string RegisteredModularityBInstance::InstancesPath =
//		"../bipartite_instances/";

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

void singletize(size_t i, IndexedList & allNodes,
		ModularityBPartition & solution) {
	allNodes.fill();
	for (size_t p(0); p < i && !allNodes.empty(); ++p) {
		size_t const id(allNodes.pop_random());
		if (solution.sizeOfLabel(solution.label(id)) > 1) {
			solution.shift(id, solution.getUnUsedLabel());
			++p;
		}
	}
	solution.score() = solution.computeScore();
}

void set(IntVector const & l, ModularityBPartition & solution) {
	for (size_t i(0); i < l.size(); ++i)
		solution.shift(i, l[i]);
}

int main(int argc, char** argv) {
	Timer total;
	if (argc == 1)
		return usage();
	AvailableModularityBInstances id(
			static_cast<AvailableModularityBInstances>(atoi(argv[1]) - 1));
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
	IndexedList allNodes(instance.nV(), true);
	ModularityBPartition p(instance, instance.nV());
	
	p.score() = p.computeScore();

	//Divisor divisor(instance);	
	//IndexedList usedLabels;
	//IndexedList skipedLabels( p.usedLabels());
	//skipedLabels.clear();
	//bool stop(false);
	//while (!stop) {
	//	std::cout << "NEW LOOP" << std::endl;
	//	stop = true;
	//	usedLabels = p.usedLabels();		
	//	for (auto const & label : usedLabels) {
	//		if (!skipedLabels.contains(label)){
	//			if(divisor.run(p, label)) 
	//				stop = false;
	//			else{
	//				skipedLabels.insert(label);
	//			}
	//		std::cout << std::setw(4) << label;
	//		std::cout << std::setw(20) <<std::setprecision(10)<< p.score();
	//		std::cout << std::endl;
	//		}
	//	}
	//}
	//std::ofstream file("divisor.sol");
	//for (size_t i(0); i < p.nbObs(); ++i)
	//	file << p.label(i) << std::endl;
	//file.close();
	//exit(0);
	"divisor.sol" >> p;
	p.score() = p.computeScore();
	//	for (size_t i(0); i < p.nbObs(); ++i) {
	//		p.shift(i, i);
//	}
//	p.score() = p.computeScore();
//	std::cout << p.score() << std::endl;
//	LabelPropagation lpa(instance, p);
//	lpa();
//	/*
//	 * VNS
//	 */
	BranchAndBound branchAndBound(instance);
	branchAndBound.init();
//	Double const kMax((int) instance.nV() * 1.0);
//	size_t const iteMax(5);
//	IntVector best(p.labels());
//	Double bestScore(p.score());
//	size_t ite(0);
//	std::cout << std::setw(4) << ite;
//	std::cout << std::setw(4) << 0;
//	std::cout << std::setw(4) << p.nbLabels();
//	std::cout << std::setw(15) << p.score();
//	std::cout << std::endl;
//	do {
//		size_t k(0);
//		++ite;
//		do {
//			++k;
//			singletize(k, allNodes, p);
//			branchAndBound.master().add(p);
//			lpa();
////			std::cout << std::setw(4) << ite;
////			std::cout << std::setw(4) << k;
////			std::cout << std::setw(4) << p.nbLabels();
////			std::cout << std::setw(15) << p.score();
////			std::cout << std::endl;
//			branchAndBound.master().add(p);
//			if (bestScore + 1e-10 < p.score()) {
//				bestScore = p.score();
//				best = p.labels();
//				std::cout << std::setw(4) << ite;
//				std::cout << std::setw(4) << k;
//				std::cout << std::setw(4) << p.nbLabels();
//				std::cout << std::setw(15) << p.score();
//				std::cout << std::setw(8)
//						<< branchAndBound.master().columns().size();
//				std::cout << std::endl;
//				k = 0;
//			} else {
//				set(best, p);
//			}
//
//		} while (k <= kMax);
//	} while (ite <= iteMax);
//	std::cout << "VNS ENDED" << std::endl;
//	set(best, p);
	branchAndBound.master().add(p);
	branchAndBound.run();
	branchAndBound.writeSolution();
	std::cout << "program run in " << std::setprecision(10) << total.elapsed()
			<< std::endl;
	return 0;
}

