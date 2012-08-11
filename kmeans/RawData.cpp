/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "RawData.hpp"

RectMatrix ReadRawData(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	size_t n;
	size_t m;
	file >> n;
	file >> m;
	Double v;
	RectMatrix matrix(n, m);
	for (size_t i(0); i < n; ++i) {
		for (size_t j(0); j < m; ++j) {
			file >> v;
			matrix.set(i, j, v);
		}
	}
	return matrix;
}
