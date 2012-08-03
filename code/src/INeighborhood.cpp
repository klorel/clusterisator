/*
 * INeighborhood.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "INeighborhood.hpp"
#include "Data.hpp"
#include "IndexedList.hpp"

Data & INeighborhood::data() {
	return _data;
}
Data const & INeighborhood::data() const {
	return _data;
}
ICriterion const & INeighborhood::criterion() const {
	return _criterion;
}

IndexedList const & INeighborhood::nodes() const {
	return _nodes;
}
IndexedList const & INeighborhood::labels() const {
	return _labels;
}
INeighborhood::INeighborhood(Data& data, ICriterion const & criterion,
		IndexedList const & nodes, IndexedList const & labels) :
		_data(data), _criterion(criterion), _nodes(nodes), _labels(labels) {
}
INeighborhood::~INeighborhood() {

}
