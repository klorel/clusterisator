/*
 * RandIndex.hpp
 *
 *  Created on: 20 août 2012
 *      Author: manuel
 */

#ifndef RANDINDEX_HPP_
#define RANDINDEX_HPP_

#include "../src/IMesure.hpp"
#include "../src/common.h"

class RandIndex: public IMesure {
public:
	RandIndex();
	virtual ~RandIndex();
public:

	Double compute(IPartition const & real, IPartition const & candidate);
private:


};

#endif /* RANDINDEX_HPP_ */
