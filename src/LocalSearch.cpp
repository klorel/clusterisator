/*
 * LocalSearch.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "LocalSearch.hpp"
#include "INeighborhood.hpp"

LocalSearch::LocalSearch(Data & data, ICriterion & criterion,
		INeighborhood & neighborhood) :
		_data(data), _criterion(criterion), _neighborhood(neighborhood) {

}

LocalSearch::~LocalSearch() {
}

bool LocalSearch::run() {
	bool improvementDone(false);
	bool stop(false);
	do {
		if (loop())
			improvementDone = true;
		else
			stop = true;
	} while (!stop);
	return improvementDone;
}

bool LocalSearch::loop() {
	bool improvementDone(false);
	for (auto const & seed : _neighborhood) {
		if (_neighborhood.findBest(seed))
			improvementDone = true;
	}
	return improvementDone;

}
