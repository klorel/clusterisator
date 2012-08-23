/*
 * KMeansAlgo.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef KMEANSALGO_HPP_
#define KMEANSALGO_HPP_

#include "src/common.h"
#include "src/KMPartition.hpp"
#include "src/RectMatrix.hpp"
#include "src/Partition.hpp"
#include "src/Timer.hpp"
#include "src/KMInput.hpp"

class KMAlgo {
public:
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;
public:
	static Double ComputeMssc(IPartition const &, KMInstance const &);
	template<bool isTraceOn = true> static bool KMeans(KMInput & input);
	template<bool isTraceOn = true> static bool HMeans(KMInput & input);

	template<bool isTraceOn = true> static bool HKMeans(KMInput & input);

	static bool Singleton(KMInput & input);
private:
	static bool HMeansLoop(KMInput & input);
private:
	static IntVector _Buffer;
	static std::string const HMEANS;
	static std::string const KMEANS;

};
template<bool isTraceOn> inline bool KMAlgo::HKMeans(KMInput & input) {
	bool success(false);
	if (HMeans<isTraceOn>(input))
		success = true;
	if (KMeans<isTraceOn>(input))
		success = true;
	return success;
}
template<bool isTraceOn> inline bool KMAlgo::KMeans(KMInput & input) {
	bool improvement(false);
	input.computeDistances();
	do {
		++input.ite();
		improvement = false;
		for (size_t obs(0); obs < input.nbObs(); ++obs) {
			std::pair<size_t, Double> delta(input.getBest(obs));
			if (delta.second < 0) {
				input.cost() += delta.second;
				input.shift(obs, delta.first);
				improvement = true;
			}
		}
		if (improvement && isTraceOn)
			input.out(KMEANS);

	} while (improvement);
	return improvement;
}
template<bool isTraceOn> inline bool KMAlgo::HMeans(KMInput & input) {

	bool stop(false);
	input.computeDistances();
	input.fillModified();
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
			input.out(HMEANS);

	} while (!stop);
	return true;
}
#endif /* KMEANSALGO_HPP_ */

