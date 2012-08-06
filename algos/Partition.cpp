/*
 * Partition.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Partition.hpp"

Partition::Partition(size_t const & n, size_t const & m) :
		IntVector(n, m) {

}

Partition::Partition(std::string const & fileName) :
		IntVector() {
	std::ifstream file(fileName.c_str());
	std::string line;
	size_t j(-1);
	while (file >> j) {
		push_back(j);
	}
}
Partition::Partition(IPartition const & p) :
		IntVector(p.nbNodes()) {
	for (size_t i(0); i < size(); ++i)
		label(i) = p.label(i);

}

Partition::~Partition() {
}

size_t Partition::nbNodes() const {
	return size();
}
size_t & Partition::label(size_t const & node) {
	return (*this)[node];
}
size_t Partition::label(size_t const & node) const {
	return (*this)[node];
}
