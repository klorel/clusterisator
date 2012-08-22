/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "../src/common.h"
#include "../src/Partition.hpp"

#include "../src/KMAlgo.hpp"
#include "../src/KMInstance.hpp"
#include "../src/KMConstraints.hpp"
#include "../src/KMPartition.hpp"

#include "../src/Number.hpp"
#include "../src/RegisteredInstance.hpp"
#include "../src/RandIndex.hpp"

class Launcher: public ILauncher {
public:
	void run(AvailableInstances id) {

		RegisteredInstance instance(id);
		OUT<< std::setw(25) << std::left << instance.name;
		Partition real(instance.real());
		size_t const k(real.nbLabels());
		OUT<< std::setw(25) <<std::setprecision(15) << std::right<<KMAlgo::ComputeMssc(real,instance) << "\n";

		Agregations agregations;
		instance.buildMustLink(agregations);

		KMInstance instance2(instance, agregations);
		KMPartition partition2(instance2, k);

//		partition2.random(k);
		for (size_t i(0); i < instance.nbObs(); ++i)
			partition2.shift(agregations.newIds[i], real.label(i));
//		partition2.set(real);
		Input input(partition2);
		OUT<< std::setw(25) <<std::setprecision(15) << std::right<<KMAlgo::ComputeMssc(partition2,instance2) << "\n";
		input.headers();
		KMAlgo::HMeans(input);
		KMAlgo::KMeans(input);

		Partition candidate(real);
		for (size_t i(0); i < real.nbObs(); ++i)
			candidate.shift(i, partition2.label(agregations.newIds[i]));
		OUT<< std::setw(25) << std::left << instance.name;
		OUT<< std::setw(25) << std::left << RandIndex().compute(real, real);
		OUT<< std::setw(25) << std::left << RandIndex().compute(real, candidate);
		OUT<< "\n";
	}

	virtual ~Launcher() {
	}
};

int main(int argc, char ** argv) {
	Number::SetSeed(0);
//	RunAllFrom<AvailableInstances::wine> f;
//	f.go<Launcher>();

//	Launcher().run(AvailableInstances::iris);

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
