/*
 * HMeans.hpp
 *
 *  Created on: 24 août 2012
 *      Author: manuel
 */

#ifndef HMEANS_HPP_
#define HMEANS_HPP_

#include "src/common.h"
#include "src/KMInput.hpp"

template<bool isTraceOn>
class HMeans {
public:
	bool operator()(KMInput & input) {
		bool stop(false);
		input.computeDistances();
		input.fillModified();
		if (isTraceOn)
			input.out("HMEANS");
		do {
			++input.ite();
			HMeansLoop(input);
			if (input.emptyMoves()) {
				stop = true;
			} else {
				input.clearModified();
				if (!input.applyMoves())
					stop = true;

			}
			input.computeDistances();
			Singleton(input);
			input.computeDistances();

			if (!stop && isTraceOn)
				input.out("HMEANS");

		} while (!stop);
		return true;
	}
private:
	static bool Singleton(KMInput & input);
	static bool HMeansLoop(KMInput & input);
	static IntVector _Buffer;

};
template<bool isTraceOn> IntVector HMeans<isTraceOn>::_Buffer = IntVector();
// we just need to test :
// - obs of which label was modified
template<bool isTraceOn> inline bool HMeans<isTraceOn>::HMeansLoop(
		KMInput & input) {
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
template<bool isTraceOn> inline bool HMeans<isTraceOn>::Singleton(
		KMInput & input) {
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

#endif /* HMEANS_HPP_ */
