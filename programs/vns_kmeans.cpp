/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "common.h"
#include "Partition.hpp"

#include "KMAlgo.hpp"
#include "KMInstance.hpp"
#include "KMConstraints.hpp"
#include "KMPartition.hpp"

#include "Number.hpp"
#include "RegisteredKMInstance.hpp"
#include "RandIndex.hpp"

#include "Vns.hpp"

struct DoubleComparator {
	bool operator()(Double a, Double b) const {
		return a - b < -1e-6;
	}
};

//		std::cout << "param sizeN := " << instance.nbObs() << ";\n";
//		std::cout << "param sizeK := " << instance.nbObs() << ";\n";
//		std::cout << "param sizeD := " << instance.nbAtt() << ";\n";
//		std::cout << "param X :=\n";
//		for (size_t i(0); i < instance.nbObs(); ++i)
//			for (size_t d(0); d < instance.nbAtt(); ++d)
//				std::cout << i + 1 << " " << d + 1 << " " << instance.get(i, d)
//						<< "\n";
//		std::cout << ";\n";
//		exit(0);

class Launcher: public ILauncher {
public:
	void run(AvailableInstances id, size_t k) {

		RegisteredKMInstance instance(id);
		instance.out();

		Aggregations aggregations;
		instance.buildMustLink(aggregations);
//
		KMInstance instance2(instance, aggregations);
		KMInput input(instance2, k);

		size_t const p(1);
		for (size_t i(0); i < p; ++i) {
			input.random(0);
			Vns vns(input);
			vns.run<HKMeans, true>(5, 10000);
		}
	}

	virtual ~Launcher() {
	}
};

void usage() {
	std::cout << "Available instances : \n";
	for (size_t i(0); i < AvailableInstances::SIZE; ++i) {
		AvailableInstances id(static_cast<AvailableInstances>(i));
		RegisteredKMInstance instance(id);
		std::cout << std::setw(3) << i;
		std::cout << " : ";
		std::cout << std::setw(30) << std::left << instance.name << std::right;

		std::cout << "\n";
	}
	std::cout << "<exe> <id of selected instance> <number of labels>\n";

}

int main(int argc, char ** argv) {
	//	RunAllFrom<AvailableInstances::wine> f;
	//	f.go<Launcher>();
	if (argc == 1)
		usage();
	else {

		size_t const i(atoi(argv[1]));
		size_t const k(atoi(argv[2]));
		if (i < AvailableInstances::SIZE) {
			AvailableInstances id(static_cast<AvailableInstances>(i));
			Launcher().run(id, k);
		}
	}
	return 0;
}

//std::string const dataFileName(argv[1]);
//size_t const k(atoi(argv[2]));
//
//Number::SetSeed(0);
//
//for (size_t i(0); i < AvailableInstances::SIZE; ++i) {
//	AvailableInstances id(static_cast<AvailableInstances>(i));
//	GetInstance<id> instance;
//	std::cout << "TREATING : " << instance.Name << "\n";
//	Aggregations aggregations;
//
//	instance.buildMustLink(aggregations);
//	// agrégation
//	KMInstance instance2(instance, aggregations);
//	KMPartition partition2(instance2, k);
//	partition2.random(k);
//	KMAlgo kmeans2(partition2);
//
//	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
////		kmeans2.hMeans(600);
////		kmeans2.kMeans(600);
//}
//return 0;
