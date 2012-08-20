/*
 * IMesure.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef IMESURE_HPP_
#define IMESURE_HPP_

#include "../src/common.h"

class IPartition;
class IMesure {
public:
	virtual Double compute(IPartition const & real,
			IPartition const & candidate) = 0;
	virtual ~IMesure();
};

inline IMesure::~IMesure() {
}

#endif /* IMESURE_HPP_ */
