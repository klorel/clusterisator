/*
 * kmeans_checker.cpp
 *
 *  Created on: 17 août 2012
 *      Author: manuel
 */

#include "../src/common.h"
#include "../src/Partition.hpp"

#include "../src/KMInstance.hpp"
#include "../src/KMPartition.hpp"
#include "../src/KMAlgo.hpp"

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
	std::string const partFileName(argv[2]);
	KMInstance instance;
	instance.readData(dataFileName);
	size_t const NB_OBS(instance.nbObs());
	if (argc > 3) {
		std::string const ctrsFileName(argv[3]);
		instance.readConstraints(ctrsFileName);
	}
	Partition partition(NB_OBS);
	partFileName >> partition;
	if (instance.feasible(partition)) {
		OUT<<"MSSC : "<< std::setprecision(15)<<KMAlgo::ComputeMssc(partition, instance)<<"\n";
	}

//	KMPartition kmpartition(instance);
//	partFileName >> kmpartition;
//	if (instance.feasible(kmpartition)) {
//		OUT<<"MSSC : "<< std::setprecision(15)<<mssc(kmpartition, instance)<<"\n";
//	}
	return 0;
}
