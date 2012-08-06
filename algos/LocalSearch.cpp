/*
 * LocalSearch.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "LocalSearch.hpp"
#include "INeighborhood.hpp"
#include "IExtendedPartition.hpp"
#include "ICriterion.hpp"

LocalSearch::LocalSearch(INeighborhood & neighborhood) :
		_neighborhood(neighborhood) {

}

LocalSearch::~LocalSearch() {
}

void LocalSearch::init() {
	init(_score, _scores);
}
void LocalSearch::init(double & score, DoubleVector & scores) const {
	score = 0;
	scores.assign(_neighborhood.data().nbNodes(), 0);
	for (auto const & label : _neighborhood.data().used()) {
		scores[label] = _neighborhood.criterion().eval(_neighborhood.data(),
				label);

		score += scores[label];

	}
}
void LocalSearch::check() const {
	double score;
	DoubleVector scores;
	init(score, scores);
	std::cout << "score = " << _score << "\n";
	double const absolute_score(
			_neighborhood.criterion().eval(_neighborhood.data()));
	if (fabs(absolute_score - score) > 1e-10) {
		std::cout << "Wrong  score, " << score << " != " << absolute_score
				<< "\n";
		exit(0);
	}
	if (fabs(score - _score) > 1e-10) {
		std::cout << "Wrong _score, " << _score << " != " << score << "\n";
		exit(0);
	}
	for (size_t i(0); i < _neighborhood.data().nbNodes(); ++i)
		if (fabs(scores[i] - _scores[i]) > 1e-10) {
			std::cout << "Wrong _scores[" << i << "], " << _scores[i] << " != "
					<< scores[i] << "\n";
			exit(0);
		}

}
bool LocalSearch::run() {
	init();
	bool improvementDone(false);
	bool stop(false);
	size_t i(0);
	do {
		if (loop())
			improvementDone = true;
		else
			stop = true;
		std::cout << "loop " << ++i << "\n";
	} while (!stop);
	return improvementDone;
}

bool LocalSearch::loop() {
	bool improvementDone(false);
	for (auto const & seed : _neighborhood) {
		check();
		if (_neighborhood.findBest(seed, _score, _scores))
			improvementDone = true;

	}
	return improvementDone;

}
