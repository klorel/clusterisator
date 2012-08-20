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

#include "../src/Env.hpp"
#include "../src/Number.hpp"
#include "../src/RegisteredInstance.hpp"

class ILauncher {
public:
	virtual void run(AvailableInstances id)=0;
	virtual ~ILauncher() {
	}
};

class Launcher: public ILauncher {
public:
	void run(AvailableInstances id) {
		RegisteredInstance instance(id);
//		KMInstance instance(Info::Get(id));
		std::cout << std::setw(25) << std::left << instance.name;
		Partition real(instance.real());
		size_t const k(real.nbLabels());
		OUT<< std::setw(25) <<std::setprecision(15) << std::right<<KMAlgo::ComputeMssc(real,instance) << "\n";
		Agregations agregations;
		instance.buildMustLink(agregations);
		// agrégation
		KMInstance instance2(instance, agregations);
		KMPartition partition2(instance2, k);

		partition2.random(k);
		KMAlgo kmeans2(partition2);
		std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
		kmeans2.headers(OUT);
		kmeans2.hMeans(600);
		kmeans2.kMeans(600);
	}

	virtual ~Launcher() {
	}
};

template<AvailableInstances No> class RunAllFrom {
public:
	enum {
		value = No
	};

	template<class LaunchT>
	void go() {
		// on lance ce run
		LaunchT().run(No);
		// on lance le suivant
		RunAllFrom<static_cast<AvailableInstances const>(No + 1)> t;
		t.go<LaunchT>();
	}
};

// termination condition
template<> class RunAllFrom<AvailableInstances::SIZE> {
public:
	enum {
		value = AvailableInstances::SIZE
	};
	template<class LaunchT>
	void go() {

	}
};

int main(int argc, char ** argv) {
	Number::SetSeed(0);

//	RunAllFrom<AvailableInstances::wine> f;
//	f.go<Launcher>();

	Launcher().run(AvailableInstances::iris);

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
