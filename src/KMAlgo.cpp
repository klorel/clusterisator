/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMAlgo.hpp"

std::string const KMAlgo::HMEANS = "H-MEANS";
std::string const KMAlgo::KMEANS = "K-MEANS";

IntVector KMAlgo::_Buffer = IntVector();

Double KMAlgo::ComputeMssc(IPartition const & x, KMInstance const & instance) {
	RectMatrix centers(x.maxNbLabels(), instance.nbAtt());
	centers.assign(0);
	DoubleVector weights(x.maxNbLabels(), Zero<Double>());
	for (auto const & l : x.used()) {
		for (auto const & i : x.list(l)) {
			weights[l] += instance.weight(i);
			for (size_t d(0); d < instance.nbAtt(); ++d)
				centers.plus(l, d, instance.get(i, d) * instance.weight(i));
		}
	}
	Double result(Zero<Double>());
	for (size_t i(0); i < instance.nbObs(); ++i) {
		size_t const l(x.label(i));
		for (size_t d(0); d < instance.nbAtt(); ++d)
			result += instance.weight(i)
					* std::pow(
							instance.get(i, d) - centers.get(l, d) / weights[l],
							2);

	}

	return result;
}

// we just need to test :
// - obs of which label was modified
bool KMAlgo::HMeansLoop(KMInput & input) {
	input.clearMoves();
	for (size_t i(0); i < input.nbObs(); ++i) {
		std::pair<size_t, Double> const k(input.getClosest(i));
		if (k.first != input.label(i)) {
			input.moves().push_back(std::make_pair(i, k.first));
			//			assert(_input.getDelta(i,k.first)<0);
		}
	}
	return !input.moves().empty();
}
bool KMAlgo::Singleton(KMInput & input) {
	_Buffer.clear();
	//	PushBack(input.unUsed(), _Buffer,
	//			input.getK() - input.nbLabels());
	PushBack(input.unUsed(), _Buffer);

	assert(_Buffer.size() == input.unUsed().size());
	assert(_Buffer.size() + input.nbLabels() == input.getK());

	while (!_Buffer.empty()) {
		assert(!input.isUsed(_Buffer.back()));
		assert(input.distances().begin()->first>Zero<Double>());
		input.shift(input.distances().begin()->second, _Buffer.back());
		input.cost() -= input.distances().begin()->first;
		assert(input.isUsed(_Buffer.back()));
		_Buffer.pop_back();

		input.distances().erase(input.distances().begin());
	}
	return true;
}
