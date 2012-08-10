/*
 * IPartition.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef I_EXTENDED_PARTITION_HPP_
#define I_EXTENDED_PARTITION_HPP_

#include "common.h"
#include "IPartition.hpp"

class IGraphPartition: public IPartition {
public:
	// the number of edges
	virtual size_t nbLinks() const = 0;
	// the degree of a label
	virtual double const & degreeOfLabel(size_t const & label) const = 0;
	virtual double & degreeOfLabel(size_t const & label)= 0;
public:
	virtual void intra(size_t const & node, DoubleVector & values,
			bool const & reset) = 0;
	virtual double intra(size_t const & label) = 0;
	virtual double intra(size_t const & node, size_t const & label) = 0;
public:
	IGraphPartition(ILinks const & iLinks);
	virtual ~IGraphPartition();

	ILinks const & graph() const;
private:
	ILinks const & _iGraph;
};

inline IGraphPartition::IGraphPartition(ILinks const & iLinks) :
		_iGraph(iLinks) {

}
inline IGraphPartition::~IGraphPartition() {
}

inline ILinks const & IGraphPartition::graph() const {
	return _iGraph;
}

#endif /* I_EXTENDED_PARTITION_HPP_ */
