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
#include "../src/GetInstance.hpp"

template<AvailableInstances id> void launch() {
	GetInstance<id> instance;
	std::cout << std::setw(25) << std::left << instance.name << std::right;
	Partition real(instance.real());
	OUT<< std::setprecision(15) << KMAlgo::ComputeMssc(real,instance) << "\n";
//	Agregations agregations;
//	instance.buildMustLink(agregations);
//	// agrégation
//	KMInstance instance2(instance, agregations);
//	KMPartition partition2(instance2, k);
//	partition2.random(k);
//	KMAlgo kmeans2(partition2);
//
//	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
}

template<AvailableInstances No> struct run {
	enum {
		value = No
	};
	void go() {
		// on lance ce run
		launch<No>();
		// on lance le suivant
		run<static_cast<AvailableInstances const>(No + 1)> t;
		t.go();
	}
};

// termination condition
template<> struct run<AvailableInstances::SIZE> {
	enum {
		value = AvailableInstances::SIZE
	};
	void go() {

	}
};

int main(int argc, char ** argv) {
	Number::SetSeed(0);

	run<AvailableInstances::wine> f;
	f.go();
//	launch<wine>();
//	launch<wine1>();
//	launch<wine2>();
//	launch<balance_scale>();
//	launch<balance_scale1>();
//	launch<balance_scale2>();
//	launch<iris>();
//	launch<iris1>();
//	launch<iris2>();
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
