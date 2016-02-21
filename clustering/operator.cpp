/*
 * operator.cpp
 *
 *  Created on: 17 août 2012
 *      Author: manuel
 */

#include "common.h"
#include "Partition.h"

void operator>>(std::string const & fileName, Partition & partition) {
	std::ifstream file(fileName.c_str());
	if (!file.good()) {
		std::cout << "unable to open : " << fileName << "\n";
	} else {
		file >> partition;
	}
	file.close();
}

void operator>>(std::istream & stream, Partition & partition) {
	std::map<std::string, IntSet> codes;
	size_t i(0);
	std::string code;
	while (stream >> code) {
//		std::cout << i << " in " << code << "\n";
		codes[code].insert(i);
		++i;
	}
//	std::cout << codes.size() << " labels\n";
//	std::cout << i << " observations\n";
	partition.oneLabel(i, codes.size());
	i = 0;
	for (auto const & s : codes) {
//		std::cout << "label " << i << "\n";
		for (auto const & obs : s.second)
			partition.shift(obs, i);
		++i;
	}
//	exit(0);
}

void operator>>(std::istream & stream, IntVector & values) {
	values.clear();
	size_t value(0);
	values.reserve(10000);
	while (stream >> value)
		values.push_back(value);

}
