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
	Partition partition(instance.nbObs(), k);
	partition.random(k);

	Agregations agregations;
	IntVector newIds;

//	for (size_t p(0); p < 5; ++p) {

	instance.mustLink(0, 1);
	partition.shift(0, partition.label(1));
//	}
//	instance.mustLink(0, 2);
//	instance.mustLink(0, 3);

//	partition.shift(2, partition.label(0));
//	partition.shift(3, partition.label(0));

	instance.buildMustLink(agregations, newIds);
//	for (auto const & s : agregations)
//		DisplayContainer(std::cout, s);
//	for (size_t i(0); i < instance.nbObs(); ++i)
//		newIds[i] = i;

	KMInstance instance2(instance, agregations, newIds);
	std::cout << std::setprecision(15) << instance2.cst() << "\n";
//	for (size_t i(0); i < instance2.nbObs(); ++i) {
//		std::cout << instance2.weight(i) << "\n";
//		for (size_t d(0); d < instance2.nbAtt(); ++d)
//			std::cout << instance2.get(i, d) << " ";
//		std::cout << "\n";
//	}

//	assert(partition2.nbLabels()==k);

//	std::cout << instance;
//	std::cout << instance2;

	KMAlgo kmeans(instance, k);
	kmeans.set(partition);
	std::cout << std::setprecision(15) << kmeans.computeCost() << "\n";

	KMAlgo kmeans2(instance2, k);
	RectMatrix r(k, instance.nbAtt());
	kmeans2.computeCenters(r);
	assert(r==kmeans2.centers());
	for (size_t i(0); i < instance.nbObs(); ++i) {
		kmeans2.shift(newIds[i], partition.label(i));
	}
	kmeans2.computeCenters(r);

	kmeans.shift(0, 1);
	kmeans.shift(1, 1);
	std::cout << std::setprecision(15) << kmeans.computeCost() << "\n";
	std::cout << "--------------\n";

	assert(r==kmeans2.centers());
	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
	std::cout << std::setprecision(15)
			<< kmeans2.computeCost() + kmeans2.getDelta(newIds[0], 0, 1)
			<< "\n";
	kmeans2.shift(newIds[0], 1);
	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
//	assert(instance.data()==instance2.data());
//	std::cout << kmeans.centers();
//	std::cout << instance;
//	std::cout << kmeans2.centers();
//	std::cout << instance2;
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
