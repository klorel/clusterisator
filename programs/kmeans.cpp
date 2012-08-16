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

#include "../src/Env.hpp"
#include "../src/Number.hpp"

int main(int argc, char ** argv) {
	std::string const dataFileName(argv[1]);
	size_t const k(atoi(argv[2]));

	KMInstance instance;
	instance.readData(dataFileName);

	Number::SetSeed(argc > 3 ? atoi(argv[3]) : 0);
	Partition partition(instance.nbObs(), k);
	partition.random(k);

	Agregations agregations;
	IntVector newIds;
	instance.mustLink(0, 1);
	instance.mustLink(0, 2);
	instance.mustLink(0, 3);
	partition.shift(1, partition.label(0));
	partition.shift(2, partition.label(0));
	partition.shift(3, partition.label(0));

	instance.buildMustLink(agregations, newIds);

	KMInstance instance2(instance, agregations, newIds);
	Partition partition2(instance2.nbObs(), k);
	for (size_t i(0); i < instance.nbObs(); ++i) {
		partition2.shift(newIds[i], partition.label(i));
	}

	KMAlgo kmeans(instance, k);
	kmeans.set(partition);
	std::cout << kmeans.computeCost() << "\n";

	KMAlgo kmeans2(instance2, k);
	kmeans2.set(partition2);
	std::cout << kmeans2.computeCost() << "\n";

	//	kmeans.run(600);

//	kmeans.run2();
//	OUT<< data;
//	if (argc != 3) {
//		std::cout << "kmeans <graphFile> <k>\n";
//		return 0;
//	}
//	std::string dataFileName(argv[1]);
//	std::string partitionName(argv[2]);
//
//	Partition partition;
//	IBuilder::Get<IPartition>(partition, partitionName);
	return 0;
}
