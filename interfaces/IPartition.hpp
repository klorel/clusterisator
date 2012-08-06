/*
 * IPartition.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef IPARTITION_HPP_
#define IPARTITION_HPP_

class IPartition {
public:
	virtual size_t nbNodes() const = 0;
	virtual size_t & label(size_t const & node) = 0;
	virtual size_t label(size_t const & node) const = 0;

	virtual ~IPartition();
};

inline IPartition::~IPartition() {
}
#endif /* IPARTITION_HPP_ */
