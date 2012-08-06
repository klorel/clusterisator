/*
 * Shifter.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "Shifter.hpp"
#include "IndexedList.hpp"
#include "ICriterion.hpp"

Shifter::Shifter(IExtendedPartition& data, ICriterion const & criterion,
		IndexedList const & nodes, IndexedList const & labels) :
		INeighborhood(data, criterion, nodes, labels) {

}

Shifter::~Shifter() {

}
Shifter::const_iterator Shifter::begin() const {
	return nodes().begin();
}

Shifter::const_iterator Shifter::end() const {
	return nodes().end();
}

size_t Shifter::random() const {
	return nodes().random();
}
bool Shifter::findFirst(size_t const & node) {
	bool improvement(false);
	return improvement;
}

bool Shifter::findBest(size_t const & node, double & bestScore,
		DoubleVector & scores) {
	bool improvement(false);
	size_t const from(data().label(node));

	double candidateScore(bestScore);
	double bestFromScore(bestScore);
	double bestToScore(bestScore);
	size_t bestTo(from);

	candidateScore -= scores[from];

	for (auto const & to : labels()) {
		if (to != from) {
			candidateScore -= scores[to];
			data().shift(node, to);
			double const newFrom(criterion().eval(data(), from));
			double const newTo(criterion().eval(data(), to));
			data().shift(node, from);

			candidateScore += newFrom;
			candidateScore += newTo;
			if (criterion().isBetter(bestScore, candidateScore)) {
				improvement = true;
				bestScore = candidateScore;
				bestFromScore = newFrom;
				bestToScore = newTo;
				bestTo = to;
			}
			candidateScore -= newFrom;
			candidateScore -= newTo;
			candidateScore += scores[to];
		}
	}

	if (improvement) {
//		std::cout << node << " : " << from << " --> " << bestTo << "\n";
		data().shift(node, bestTo);
		scores[from] = bestFromScore;
		scores[bestTo] = bestToScore;

		bestScore = candidateScore;
	}
	return improvement;
}

void Shifter::findAny(size_t const & node) {

}
