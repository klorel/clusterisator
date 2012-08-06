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
// recherche le premier movement améliorant, retourne true si un mouvement améliorant est déterminé
bool Shifter::findFirst(size_t const & node) {
	bool improvement(false);
	return improvement;
}
// recherche le meilleur mouvement améliorant, retourne true si un mouvement améliorant est déterminé
bool Shifter::findBest(size_t const & node, double & score,
		DoubleVector & scores) {
	bool improvement(false);
	size_t const from(data().label(node));
	double bestScore(score);
	double bestFromScore(score);
	double bestToScore(score);
	size_t bestTo(from);

	bestScore -= scores[from];

	for (auto const & to : labels()) {
		if (to != from) {
			bestScore -= scores[to];
			data().shift(node, to);
			double const newFrom(criterion().eval(data(), from));
			double const newTo(criterion().eval(data(), to));

			bestScore += newFrom;
			bestScore += newTo;
			if (criterion().isBetter(bestScore, score)) {
				improvement = true;
				score = bestScore;
				bestFromScore = newFrom;
				bestToScore = newTo;
				bestTo = to;
			}
			bestScore -= newFrom;
			bestScore -= newTo;
		}
	}
	if (improvement) {
		data().shift(node, bestTo);
		scores[from] = bestFromScore;
		scores[bestTo] = bestToScore;
		score = bestScore;
	}
	return improvement;
}
// applique n'importe quel mouvement
void Shifter::findAny(size_t const & node) {

}
