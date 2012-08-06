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
	LocalSearch(INeighborhood &);
	virtual ~LocalSearch();

public:
	bool run();
private:
	void init(double & score, DoubleVector & scores) const;
	void init();
	bool loop();
	void check() const;
private:
	INeighborhood & _neighborhood;

	double _score;
	DoubleVector _scores;
};

#endif /* LOCALSEARCH_HPP_ */
