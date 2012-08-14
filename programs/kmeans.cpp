/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "../utils/common.h"
#include "../data/Partition.hpp"

#include "../kmeans/KMAlgo.hpp"
#include "../kmeans/RawData.hpp"

#include "../utils/Env.hpp"
#include "../utils/Number.hpp"

int main(int argc, char ** argv) {
	std::string const dataFileName(argv[1]);
	size_t const k(atoi(argv[2]));

	RectMatrix data(ReadRawData(dataFileName));
	KMAlgo kmeans(data);
	kmeans.set(k);
	Number::SetSeed(argc > 3 ? atoi(argv[3]) : 0);
	kmeans.random();
	kmeans.run(600);

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
