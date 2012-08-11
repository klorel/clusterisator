/*
 * Shifter.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "Shifter.hpp"
#include "IndexedList.hpp"
#include "ICriterion.hpp"

Shifter::Shifter(IGraphPartition& data, ICriterion const & criterion,
		IndexedList const & nodes, IndexedList const & labels) :
		INeighborhood(data, criterion, nodes, labels) {
	_intra.assign(1, DoubleVector(data.nbObs(), 0));

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

bool Shifter::findBest(size_t const & node, Double & score) {
	size_t const from(data().label(node));
	size_t bestTo(from);
	DoubleVector & intra(_intra[0]);
	if (data().sizeOfLabel(from) > 1) {
		data().intra(node, intra, true);
		Double bestDelta(0);
		for (auto const & to : labels()) {
			if (criterion().canShift(data(), node, to)) {
				Double const delta(
						criterion().getDeltaShift(data(), node, to, intra));
				if (criterion().isBetter(delta, bestDelta)) {
					bestDelta = delta;
					bestTo = to;
				}
			}
		}
		if (bestTo != from) {
			std::cout << node << " : " << from << " --> " << bestTo << "\n";
			data().shift(node, bestTo);
			score += bestDelta;
		}
		//	FOR_EACH_CONST(node2, _data.graph().adjacentList(node)){
		//		intra[_data.label(node2.first)]=0;
		//	}
	}
	return bestTo != from;
}

void Shifter::findAny(size_t const & node) {

}
