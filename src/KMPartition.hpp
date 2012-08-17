/*
 * KMPartition.hpp
 *
 *  Created on: 17 août 2012
 *      Author: manuel
 */

#ifndef KMPARTITION_HPP_
#define KMPARTITION_HPP_

#include "../src/common.h"
#include "../src/Partition.hpp"
#include "../src/KMInstance.hpp"

class KMPartition: public Partition {
public:
	KMPartition(KMInstance const &, size_t k);
	virtual ~KMPartition();

	void computeCenters();
	void computeCenters(RectMatrix &) const;

	RectMatrix const & centers() const;

	Double cst() const;

	virtual void shift(size_t node, size_t to);

	size_t getK() const;
	template<bool isInsertion> Double getCoeff(size_t i, size_t k) const;

	Double getDistance(size_t i, size_t k) const;
	Double getDistance(size_t i) const;
private:
	KMInstance const & _input;
	RectMatrix _centers;
};

inline size_t KMPartition::getK() const {
	return _centers.getN();
}

inline Double KMPartition::getDistance(size_t i) const {
	return getDistance(i, label(i));
}
inline Double KMPartition::getDistance(size_t i, size_t k) const {
	Double result(0);
	for (size_t d(0); d < _input.nbAtt(); ++d) {
		if (labelWeight(k) == Zero<Double>())
			result += std::pow(_input.get(i, d), 2);
		else
			result += std::pow(
					_input.get(i, d) - _centers.get(k, d) / labelWeight(k), 2);
	}
//	return std::sqrt(result);
	return result;
}

template<> inline Double KMPartition::getCoeff<true>(size_t i, size_t k) const {
	return labelWeight(k) / (labelWeight(k) + obsWeight(i));
}
template<> inline Double KMPartition::getCoeff<false>(size_t i,
		size_t k) const {
	return labelWeight(k) / (labelWeight(k) - obsWeight(i));
}

inline Double KMPartition::cst() const {
	return _input.cst();
}

inline RectMatrix const & KMPartition::centers() const {
	return _centers;
}

#endif /* KMPARTITION_HPP_ */
