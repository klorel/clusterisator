/*
 * INeighborhood.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "INeighborhood.hpp"
#include "Data.hpp"
Data & INeighborhood::data() {
	return _data;
}
Data const & INeighborhood::data() const {
	return _data;
}
ICriterion const & INeighborhood::criterion() {
	return _criterion;
}

INeighborhood::INeighborhood(Data& data, ICriterion const & criterion) :
		_data(data), _criterion(criterion) {
}
INeighborhood::~INeighborhood() {

}
void INeighborhood::backTrack() {
	for (auto const p : _backTrackInfo)
		_data.shift(p.first, p.second);
}
