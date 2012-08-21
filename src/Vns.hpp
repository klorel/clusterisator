/*
 * Vns.hpp
 *
 *  Created on: 20 août 2012
 *      Author: manuel
 */

#ifndef VNS_HPP_
#define VNS_HPP_

#include "../src/common.h"
#include "../src/KMAlgo.hpp"

class Vns {
public:
	Vns(KMAlgo &);
	virtual ~Vns();
public:
	// simply moves n observations to random vertices
	void shake(size_t);
	// set instance in _algo to best
	void restart();
	//
	void save();
	//
	void run(size_t maxIte, size_t magMax);

	void out()const;
private:
	KMAlgo & _algo;
	// (point, valeur)
	std::pair<Partition, Double> _best;

	IntVector _nodes;

	size_t _ite;
	size_t _k;
	Timer _timer;
};

#endif /* VNS_HPP_ */
