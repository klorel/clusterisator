/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "KMInstance.hpp"

RectMatrix const & KMInstance::data() const {
	return _data;
}
size_t KMInstance::nbObs() const {
	return _data.getN();
}
size_t KMInstance::nbAtt() const {
	return _data.getM();
}

Double KMInstance::get(size_t i, size_t j) const {
	return _data.get(i, j);
}
Double KMInstance::weight(size_t i) const {
	return _weights[i];
}
Double & KMInstance::weight(size_t i) {
	return _weights[i];
}

DoubleVector const & KMInstance::weights() const {
	return _weights;
}
void KMInstance::readData(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	_data = RectMatrix(0, 0);
	_weights.assign(0, Zero<Double>());
	if (file.good()) {
		size_t n;
		size_t m;
		file >> n;
		file >> m;
		_data = RectMatrix(n, m);
		_weights.assign(n, Zero<Double>());
		_must = KMConstraints(n);
		_cannot = KMConstraints(n);
		Double v;
		for (size_t i(0); i < n; ++i) {
			for (size_t j(0); j < m; ++j) {
				file >> v;
				_data.set(i, j, v);
			}
		}
	} else {
		OUT<< "unable to open file "<<fileName<<"\n";
	}
}
