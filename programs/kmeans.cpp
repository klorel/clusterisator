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

//template<typename T>
//std::ostream& write(std::ostream& os, const T& value) {
//	return os.write(reinterpret_cast<const char*>(&value), sizeof(T));
//}
//
//template<typename T>
//std::istream & read(std::istream& is, T& value) {
//	return is.read(reinterpret_cast<char*>(&value), sizeof(T));
//}

int main(int argc, char ** argv) {

	std::string const dataFileName(argv[1]);
	size_t const k(atoi(argv[2]));

	KMInstance instance;
	instance.readData(dataFileName);

	Number::SetSeed(argc > 3 ? atoi(argv[3]) : 0);
	KMPartition partition(instance, k);
	assert(partition.checkWeights());

	RectMatrix r(k, instance.nbAtt());
	partition.computeCenters(r);
	assert(r == partition.centers());

	std::cout << "partition.random(k);\n";
	partition.random(k);

	//	for (size_t p(0); p < 5; ++p) {

	instance.mustLink(0, 1);
	partition.shift(0, partition.label(1));
	//	}
	KMAlgo kmeans(partition);
	std::cout << std::setprecision(15) << kmeans.computeCost() << "\n";

	Agregations agregations;
	IntVector newIds;
	instance.buildMustLink(agregations, newIds);

	KMInstance instance2(instance, agregations, newIds);
	std::cout << std::setprecision(15) << instance2.cst() << "\n";

	KMPartition partition2(instance2, k);
	for (size_t i(0); i < instance.nbObs(); ++i) {
		partition2.shift(newIds[i], partition.label(i));
	}

	std::cout << std::setprecision(15) << kmeans.computeCost() << "\n";
	kmeans.shift(0, 1);
	kmeans.shift(1, 1);
	std::cout << std::setprecision(15) << kmeans.computeCost() << "\n";
	std::cout << "--------------\n";

	KMAlgo kmeans2(partition2);
	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
	std::cout << std::setprecision(15)
			<< kmeans2.computeCost() + kmeans2.getDelta(newIds[0], 0, 1)
			<< "\n";
	kmeans2.shift(newIds[0], 1);
	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
	//

	kmeans.hMeans(600);
	kmeans2.hMeans(600);

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
