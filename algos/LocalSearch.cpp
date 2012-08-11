/*
 * LocalSearch.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "../algos/LocalSearch.hpp"
#include "../interfaces/INeighborhood.hpp"
#include "../interfaces/IGraphPartition.hpp"
#include "../interfaces/ICriterion.hpp"

LocalSearch::LocalSearch(INeighborhood & neighborhood) :
		_neighborhood(neighborhood), _score(0), _scores(
				neighborhood.data().nbObs(), 0) {

}

LocalSearch::~LocalSearch() {
}

void LocalSearch::init() {
	init(_score, _scores);
}
void LocalSearch::init(Double & score, DoubleVector & scores) const {
	score = 0;
	scores.assign(_neighborhood.data().nbObs(), 0);
	for (auto const & label : _neighborhood.data().used()) {
		scores[label] = _neighborhood.criterion().eval(_neighborhood.data(),
				label);
		score += scores[label];
	}
}
void LocalSearch::check() const {
	Double score;
	DoubleVector scores;
	init(score, scores);
	//	std::cout << "score = " << _score << "\n";
	Double const absolute_score(
			_neighborhood.criterion().eval(_neighborhood.data()));
	if (std::abs(absolute_score - score) > 1e-10) {
		std::cout << "Wrong  score, " << score << " != " << absolute_score
				<< "\n";
		exit(0);
	}
	if (std::abs(score - _score) > 1e-10) {
		std::cout << "Wrong _score, " << _score << " != " << score << "\n";
		exit(0);
	}
	for (size_t i(0); i < _neighborhood.data().nbObs(); ++i)
		if (std::abs(scores[i] - _scores[i]) > 1e-10) {
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
	display(i, std::cout << std::setw(6) << "loop");
	do {
		++i;
		if (loop()) {
			improvementDone = true;
		} else
			stop = true;
		check();
		display(i, std::cout << std::setw(6) << "loop");
	} while (!stop);

	return improvementDone;
}

bool LocalSearch::loop() {
	bool improvementDone(false);
	for (auto const & seed : _neighborhood) {
		if (_neighborhood.findBest(seed, _score)) {
			display(seed, std::cout << std::setw(6) << "node");
			check();
			improvementDone = true;
		}
	}
	return improvementDone;

}
