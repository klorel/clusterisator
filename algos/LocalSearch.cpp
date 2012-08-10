/*
 * LocalSearch.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "LocalSearch.hpp"
#include "INeighborhood.hpp"
#include "IGraphPartition.hpp"
#include "ICriterion.hpp"

LocalSearch::LocalSearch(INeighborhood & neighborhood) :
_neighborhood(neighborhood), _score(0), _scores(
		neighborhood.data().nbObs(), 0) {

}

LocalSearch::~LocalSearch() {
}

void LocalSearch::init() {
	init(_score, _scores);
}
void LocalSearch::init(double & score, DoubleVector & scores) const {
	score = 0;
	scores.assign(_neighborhood.data().nbObs(), 0);
	FOR_EACH_CONST(label, _neighborhood.data().used()) {
		scores[label] = _neighborhood.criterion().eval(_neighborhood.data(),
				label);
		score += scores[label];
	}
}
void LocalSearch::check() const {
	double score;
	DoubleVector scores;
	init(score, scores);
	//	std::cout << "score = " << _score << "\n";
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
	for (size_t i(0); i < _neighborhood.data().nbObs(); ++i)
		if (fabs(scores[i] - _scores[i]) > 1e-10) {
			std::cout << "Wrong _scores[" << i << "], " << _scores[i] << " != "
					<< scores[i] << "\n";
			exit(0);
		}

}
void LocalSearch::display(size_t const & i, std::ostream & stream) const {
	stream << std::setw(6) << i;
	stream << std::setw(15) << _score;
	stream << "\n";
}
bool LocalSearch::run() {
	init();
	std::cout << "init done\n";
	bool improvementDone(false);
	bool stop(false);
	size_t i(0);
	display(i, std::cout << std::setw(6)<<"loop");
	do {
		++i;
		if (loop()) {
			improvementDone = true;
		} else
			stop = true;
		check();
		display(i, std::cout << std::setw(6)<<"loop");
	} while (!stop);

	return improvementDone;
}

bool LocalSearch::loop() {
	bool improvementDone(false);
	FOR_EACH_CONST(seed,_neighborhood){
		if (_neighborhood.findBest(seed, _score)) {
			display(seed, std::cout << std::setw(6)<<"node");
			check();
			improvementDone = true;
		}
	}
	return improvementDone;

}
