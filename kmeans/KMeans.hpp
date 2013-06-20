/*
 * KMeans.hpp
 *
 *  Created on: 24 août 2012
 *      Author: manuel
 */

#ifndef KMEANS_HPP_
#define KMEANS_HPP_

#include "KMInput.hpp"

template<bool isTraceOn>
class KMeans {
public:
	bool operator()(KMInput & input) {
		bool improvement(false);
		input.computeDistances();
		//		IntVector tLabel(input.getK(), 0);
		//		_v.allocate(input.nbObs(), input.getK());
		//		for (size_t obs(0); obs < input.nbObs(); ++obs) {
		//			for (size_t j(0); j < input.getK(); ++j) {
		//				_v.get(obs, j) = input.getDelta(obs, j);
		//			}
		//		}
		do {
			++input.ite();
			improvement = false;
			for (size_t obs(0); obs < input.nbObs(); ++obs) {
				std::pair<size_t, Double> const delta(input.getBest(obs));
				if (delta.second < 0) {
					input.cost() += delta.second;
					input.shift(obs, delta.first);
					improvement = true;
				}
			}
			if (improvement && isTraceOn)
				input.out("KMEANS");

		} while (improvement);
		return improvement;
	}
private:
	RectMatrix _v;
};
#endif /* KMEANS_HPP_ */
