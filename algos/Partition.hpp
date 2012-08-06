/*
 * Partition.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef PARTITION_HPP_
#define PARTITION_HPP_

#include "common.h"

#include "IPartition.hpp"

class Partition: public IPartition, IntVector {
public:
	size_t nbNodes() const;
	size_t & label(size_t const & node);
	size_t label(size_t const & node) const;
	Partition(size_t const & n, size_t const & m = 0);
	Partition(IPartition const &);
	Partition(std::string  const &);
	virtual ~Partition();
};

#endif /* PARTITION_HPP_ */
