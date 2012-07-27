/*
 * LocalSearch.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef LOCALSEARCH_HPP_
#define LOCALSEARCH_HPP_

#include "common.h"

class LocalSearch {
public:
	LocalSearch(Data &, ICriterion &, INeighborhood &);
	virtual ~LocalSearch();

public:
	bool run();
private:
	bool loop();
private:
	Data & _data;
	ICriterion & _criterion;
	INeighborhood & _neighborhood;
};

#endif /* LOCALSEARCH_HPP_ */
