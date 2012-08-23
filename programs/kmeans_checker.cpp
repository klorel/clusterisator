/*
 * kmeans_checker.cpp
 *
 *  Created on: 17 août 2012
 *      Author: manuel
 */

#include "src/common.h"
#include "src/Partition.hpp"

#include "src/KMInstance.hpp"
#include "src/KMPartition.hpp"
#include "src/KMAlgo.hpp"

#include "src/Number.hpp"

//template<typename T>
//std::ostream& write(std::ostream& os, const T& value) {
//	return os.write(reinterpret_cast<const char*>(&value), sizeof(T));
//}
//
//template<typename T>
//std::istream & read(std::istream& is, T& value) {
//	return is.read(reinterpret_cast<char*>(&value), sizeof(T));
//}

void usage() {
	OUT<< "kmeans_checker <data> <partition> <constraints (optional)>\n";
	exit(0);
}

int main(int argc, char ** argv) {
	if (argc < 3) {
		OUT<< "kmeans_checker <data> <partition> <constraints (optional)>\n";
	}
	std::string const dataFileName(argv[1]);
	std::string const partFileName(argv[2]);
	std::string const ctrsFileName(argc>3?argv[3]:"");

	KMInstance instance;
	instance.readData(dataFileName);

	if (!ctrsFileName.empty()) {
		instance.readConstraints(ctrsFileName);
	}
	KMPartition kmpartition(instance);
	partFileName >> kmpartition;
	OUT<< std::left<<std::setw(20)<<"Nb observations"<<kmpartition.nbObs()<<"\n";
	OUT<< std::left<<std::setw(20)<<"Nb labels"<<kmpartition.nbLabels()<<"\n";

	if (instance.feasible(kmpartition)) {
		OUT<<std::left<<std::setw(20)<<"MSSC"<< std::setprecision(15)<<KMAlgo::ComputeMssc(kmpartition, instance)<<"\n";
		OUT<< std::right;
	} else {
		OUT<<"Infeasible solution\n";
	}
	return 0;
}
