/*
 * RawData.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef KM_INSTANCE_HPP_
#define KM_INSTANCE_HPP_

#include "../src/common.h"
#include "../src/RectMatrix.hpp"
#include "../src/KMConstraints.hpp"

class KMInstance {
public:
	void readData(std::string const &);
	void readConstraints(std::string const &);

	RectMatrix const & data() const;
	size_t nbObs() const;
	size_t nbAtt() const;
	Double get(size_t, size_t) const;

	DoubleVector const & weights()const;
	Double weight(size_t) const;
	Double & weight(size_t);
public:
	RectMatrix _data;

	DoubleVector _weights;

	KMConstraints _must;
	KMConstraints _cannot;
};

#endif /* RAWDATA_HPP_ */
