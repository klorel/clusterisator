/*
 * RawData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "KMInstance.hpp"

KMInstance::KMInstance() {
	_cst = 0;
}

// on agrège
KMInstance::KMInstance(KMInstance const & instance,
		Agregations const & agregations, IntVector& newIds) {
	allocate(agregations.size(), instance.nbAtt());
	std::fill_n(_weights.begin(), _weights.size(), Zero<Double>());

	for (size_t i(0); i < agregations.size(); ++i) {
		for (auto const & j : agregations[i]) {
			for (size_t d(0); d < nbAtt(); ++d)
				_data.plus(i, d, instance.get(j, d));

		}
		_weights[i] = static_cast<Double>(agregations[i].size());
		for (size_t d(0); d < nbAtt(); ++d)
			_data.get(i, d) /= _weights[d];
		if (_weights[i] > 1) {
			for (auto const & j : agregations[i])
				for (size_t d(0); d < nbAtt(); ++d)
					_cst += std::pow(get(i, d) - instance.get(j, d), 2);
		}
	}
	// les contraintes cannot link
	for (size_t i(0); i < instance.nbObs(); ++i) {
		for (auto const & j : instance.cannotLinks().get(i))
			cannotLink(newIds[i], newIds[j]);

	}
}

RectMatrix const & KMInstance::data() const {
	return _data;
}
size_t KMInstance::nbObs() const {
	return _data.getN();
}
size_t KMInstance::nbAtt() const {
	return _data.getM();
}

Double KMInstance::cst() const {
	return _cst;
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

void KMInstance::allocate(size_t n, size_t m) {
	_data = RectMatrix(n, m);
	_cst = Zero<Double>();
	_weights.assign(n, Zero<Double>());
	_must = KMConstraints(n);
	_cannot = KMConstraints(n);
}
void KMInstance::readData(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	if (file.good()) {
		size_t n;
		size_t m;
		file >> n;
		file >> m;
		allocate(n, m);
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

void KMInstance::buildMustLink(Agregations & result, IntVector & newIds) const {
	std::list<IntSet> agregations;
	std::vector<std::list<IntSet>::iterator> temp(nbObs(), agregations.end());
	size_t n(0);
	newIds.assign(nbObs(), -1);
	for (size_t i(0); i < nbObs(); ++i) {
		auto it(temp[i]);
		if (it == agregations.end()) {
			++n;
			agregations.push_front(IntSet());
			temp[i] = agregations.begin();
			newIds[i] = n;
		}
		for (auto const & j : _must.get(i)) {
			temp[j] = temp[i];
			newIds[j] = n;
		}
	}
	OUT<< "found "<<n<<" agregated point\n";
	result.assign(n, IntSet());
	std::copy(agregations.begin(), agregations.end(), result.begin());
}

void KMInstance::mustLink(size_t i, size_t j) {
	_must.newCtr(i, j);
}
void KMInstance::cannotLink(size_t i, size_t j) {
	_cannot.newCtr(i, j);
}
KMConstraints const & KMInstance::mustLinks() const {
	return _must;
}
KMConstraints const & KMInstance::cannotLinks() const {
	return _cannot;
}

