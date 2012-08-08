/*
 * INeighborhood.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef I_NEIGHBORHOOD_HPP_
#define I_NEIGHBORHOOD_HPP_

#include "common.h"
#include "IndexedList.hpp"
#include "IExtendedPartition.hpp"

class INeighborhood {
public:
	typedef IndexedList Container;
	typedef Container::iterator iterator;
	typedef Container::const_iterator const_iterator;
public:

	virtual const_iterator begin() const= 0;
	virtual const_iterator end() const= 0;

	virtual size_t random() const = 0;
	//
	virtual bool findFirst(size_t const &) = 0;
	//
	virtual bool findBest(size_t const & seed, double & score,
			DoubleVector & scores) = 0;
	//
	virtual void findAny(size_t const &) = 0;
public:
	IExtendedPartition & data();
	IExtendedPartition const & data() const;
	ICriterion const & criterion() const;
	IndexedList const & nodes() const;
	IndexedList const & labels() const;

	DoubleVector & scores();
	DoubleVector const & scores() const;
public:
	INeighborhood(IExtendedPartition& data, ICriterion const & criterion,
			IndexedList const & nodes, IndexedList const & labels);
	INeighborhood(IExtendedPartition& data, ICriterion const & criterion);
	virtual ~INeighborhood();
private:
	IExtendedPartition & _data;
	ICriterion const & _criterion;
	IndexedList const & _nodes;
	IndexedList const & _labels;

};

inline IExtendedPartition & INeighborhood::data() {
	return _data;
}
inline IExtendedPartition const & INeighborhood::data() const {
	return _data;
}
inline ICriterion const & INeighborhood::criterion() const {
	return _criterion;
}

inline IndexedList const & INeighborhood::nodes() const {
	return _nodes;
}
inline IndexedList const & INeighborhood::labels() const {
	return _labels;
}
inline INeighborhood::INeighborhood(IExtendedPartition& data,
		ICriterion const & criterion, IndexedList const & nodes,
		IndexedList const & labels) :
		_data(data), _criterion(criterion), _nodes(nodes), _labels(labels) {
}
inline INeighborhood::~INeighborhood() {

}

#endif /* INEIGHBORHOOD_HPP_ */