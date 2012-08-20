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
	Vns(KMAlgo & );
	virtual ~Vns();
public:
	void shake(size_t );
	void restart();
private:
	KMAlgo & _algo;
	// (point, valeur)
	std::pair<IntVector, Double> _best;
};

#endif /* VNS_HPP_ */
