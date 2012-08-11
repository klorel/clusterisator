/*
 * Acc.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef ACC_HPP_
#define ACC_HPP_

#include "../utils/common.h"
#include "../interfaces/IMesure.hpp"


class Acc: public IMesure {
public:
	Acc();
	virtual ~Acc();
public:

	Double compute(IPartition const & real,
			IPartition const & candidate) ;
};

#endif /* ACC_HPP_ */
