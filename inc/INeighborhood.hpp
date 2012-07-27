/*
 * INeighborhood.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef I_NEIGHBORHOOD_HPP_
#define I_NEIGHBORHOOD_HPP_

#include "common.h"

class INeighborhood {
public:
	typedef IntVector::iterator iterator;
	typedef IntVector::const_iterator const_iterator;
	typedef std::list<Position> BackTrackInfo;
public:

	virtual iterator begin()= 0;
	virtual const_iterator begin() const= 0;

	virtual iterator end() = 0;
	virtual const_iterator end() const= 0;
	// applique les modifs à data
	virtual void explore(size_t const &, BackTrackInfo &) = 0;
public:
	Data & data();
	Data const & data() const;
	ICriterion const & criterion();

	void backTrack();
private:
	Data & _data;
	ICriterion const & _criterion;
	BackTrackInfo _backTrackInfo;
public:
	INeighborhood(Data& data, ICriterion const & criterion);
	virtual ~INeighborhood();

};

#endif /* INEIGHBORHOOD_HPP_ */
