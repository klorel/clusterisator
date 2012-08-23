/*
 * KMeansAlgo.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef KMEANSALGO_HPP_
#define KMEANSALGO_HPP_

#include "../src/common.h"

#include "../src/KMPartition.hpp"
#include "../src/RectMatrix.hpp"
#include "../src/Partition.hpp"
#include "../src/Timer.hpp"

class Input {
public:
	Input(KMPartition & rhs);
	void out(std::string const & name) const;
	void headers();
	KMPartition & partition();
	KMPartition const& partition() const;
	Double cost() const;
	size_t & ite();
	size_t ite() const;
private:
	KMPartition & _partition;
	Timer _timer;
	size_t _ite;
	IndexedList _modifiedLabels;
	IndexedList _modifiedObs;

};

class KMAlgo {
public:
	typedef std::pair<size_t, size_t> Move;
	typedef std::vector<Move> Moves;
public:
	static Double ComputeMssc(IPartition const &, KMInstance const &);
	template<bool isTraceOn = true> static bool KMeans(Input & input);
	template<bool isTraceOn = true> static bool HMeans(Input & input);
	static bool HMeansLoop(Input & input, Moves & moves);
	static bool Singleton(Input & input);
private:
	static IntVector _Buffer;
	static std::string const HMEANS;
	static std::string const KMEANS;

};
template<bool isTraceOn> inline bool KMAlgo::KMeans(Input & input) {
	KMPartition & partition(input.partition());
	bool improvement(false);
	partition.computeDistances();
	do {
		++input.ite();
		improvement = false;
		for (size_t obs(0); obs < partition.nbObs(); ++obs) {
			std::pair<size_t, Double> delta(partition.getBest(obs));
			if (delta.second < 0) {
				partition.cost() += delta.second;
				partition.shift(obs, delta.first);
				improvement = true;
			}
		}
		if (improvement && isTraceOn)
			input.out(KMEANS);

	} while (improvement);
	return improvement;
}

inline bool KMAlgo::HMeansLoop(Input & input, Moves & moves) {
	moves.clear();
	for (size_t i(0); i < input.partition().nbObs(); ++i) {
		std::pair<size_t, Double> const k(input.partition().getClosest(i));
		if (k.first != input.partition().label(i)) {
			moves.push_back(std::make_pair(i, k.first));
			//			assert(_input.getDelta(i,k.first)<0);
		}
	}
	return !moves.empty();
}
template<bool isTraceOn> inline bool KMAlgo::HMeans(Input & input) {
	KMPartition & partition(input.partition());
	Moves moves;
	moves.reserve(partition.nbObs());

	bool stop(false);
	partition.computeDistances();

	do {
		++input.ite();
		HMeansLoop(input, moves);
		if (moves.empty()) {
			stop = true;
		} else {
			if (!partition.shift(moves))
				stop = true;

		}
		partition.computeDistances();
		Singleton(input);
		partition.computeDistances();

		if (!stop && isTraceOn)
			input.out(HMEANS);
	} while (!stop);
	return true;
}
inline bool KMAlgo::Singleton(Input & input) {
	KMPartition & partition(input.partition());

	_Buffer.clear();
	//	PushBack(partition.unUsed(), _Buffer,
	//			partition.getK() - partition.nbLabels());
	PushBack(partition.unUsed(), _Buffer);

	assert(_Buffer.size() == partition.unUsed().size());
	assert(_Buffer.size() + partition.nbLabels() == partition.getK());

	while (!_Buffer.empty()) {
		assert(!partition.isUsed(_Buffer.back()));
		assert(partition.distances().begin()->first>Zero<Double>());
		partition.shift(partition.distances().begin()->second, _Buffer.back());
		partition.cost() -= partition.distances().begin()->first;
		assert(partition.isUsed(_Buffer.back()));
		_Buffer.pop_back();

		partition.distances().erase(partition.distances().begin());
	}
	return true;
}

#endif /* KMEANSALGO_HPP_ */

