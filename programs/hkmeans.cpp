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
	std::cout << "The program launch th HK-means algorithm\n";

}

int main(int argc, char ** argv) {
	//	RunAllFrom<AvailableInstances::wine> f;
	//	f.go<Launcher>();
	if (argc < 3)
		usage();
	else {

		size_t const i(atoi(argv[1]));
		size_t const k(atoi(argv[2]));
		if (i < AvailableInstances::SIZE) {
			AvailableInstances id(static_cast<AvailableInstances>(i));
			RegisteredInstance instance(id);
			instance.out();

			Aggregations aggregations;
			instance.buildMustLink(aggregations);
			//
			KMInstance instance2(instance, aggregations);
			KMInput input(instance2, k);
			input.random(0);
			HKMeans<true>()(input);

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
