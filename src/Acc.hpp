/*
 * Acc.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef ACC_HPP_
#define ACC_HPP_

#include "../src/common.h"
#include "../src/IMesure.hpp"

class Acc: public IMesure {
public:
	Acc();
	virtual ~Acc();
public:

	Double compute(IPartition const & real, IPartition const & candidate);
};

#endif /* ACC_HPP_ */
