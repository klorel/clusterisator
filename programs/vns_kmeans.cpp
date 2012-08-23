/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
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
#include "src/RandIndex.hpp"

#include "src/Vns.hpp"

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

		RegisteredInstance instance(id);
		instance.out();

		Agregations agregations;
		instance.buildMustLink(agregations);
//
		KMInstance instance2(instance, agregations);
		KMInput input(instance2, k);

		size_t const p(1);
		for (size_t i(0); i < p; ++i) {
			//			partition.random(k);
			input.random(0);
			//		for (size_t i(0); i < instance.nbObs(); ++i)
			//			partition2.shift(agregations.newIds[i], real.label(i));
			//		partition2.set(real);

//			Input input(partition2);
//			input.headers();
//			KMAlgo::HMeans(input);
//			KMAlgo::KMeans(input);
//			kmeans2.kMeans();
			Vns vns(input);
			vns.run<true>(25, 10000);
//			results.insert(kmeans2.computeCost());
//			assert(
//					IsEqual(kmeans2.computeCost(), KMAlgo::ComputeMssc(kmeans2.partition(),instance2)));
			//		Partition candidate(real);
			//		for (size_t i(0); i < real.nbObs(); ++i)
			//			candidate.shift(i, partition2.label(agregations.newIds[i]));
			//		OUT<< std::setw(25) << std::left << instance.name;
			//		OUT<< std::setw(25) << std::left << RandIndex().compute(real, real);
			//		OUT<< std::setw(25) << std::left << RandIndex().compute(real, candidate);
			//		OUT<< "\n";
		}
	}

	virtual ~Launcher() {
	}
};

void usage() {
	std::cout << "Available instances : \n";
	for (size_t i(0); i < AvailableInstances::SIZE; ++i) {
		AvailableInstances id(static_cast<AvailableInstances>(i));
		RegisteredInstance instance(id);
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
//	Agregations agregations;
//
//	instance.buildMustLink(agregations);
//	// agrégation
//	KMInstance instance2(instance, agregations);
//	KMPartition partition2(instance2, k);
//	partition2.random(k);
//	KMAlgo kmeans2(partition2);
//
//	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
////		kmeans2.hMeans(600);
////		kmeans2.kMeans(600);
//}
//return 0;
