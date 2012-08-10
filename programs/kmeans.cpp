/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "common.h"
#include "Partition.hpp"

#include "RawData.hpp"
#include "IBuilder.hpp"

int main(int argc, char ** argv) {

	if (argc != 3) {
		std::cout << "kmeans <graphFile> <k>\n";
		return 0;
	}
	std::string dataFileName(argv[1]);
	std::string partitionName(argv[2]);

	Partition partition;
	IBuilder::Get<IPartition>(partition, partitionName);

	return 0;
}
