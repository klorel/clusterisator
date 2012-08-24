/*
 * Vns.hpp
 *
 *  Created on: 20 août 2012
 *      Author: manuel
 */

#ifndef VNS_HPP_
#define VNS_HPP_

#include "src/common.h"
#include "src/KMAlgo.hpp"

class Vns {
public:
	Vns(KMInput &);
	virtual ~Vns();
public:
	void shake(size_t);
	void restart();
	void save();

	template<template<bool T> class LocalSearchT, bool isTraceOn = false>
	void run(size_t maxIte, size_t magMax);

	void out() const;
private:
	KMInput & _input;
	// (point, valeur)
	std::pair<Partition, Double> _best;

	IntVector _nodes;

	size_t _globalIte;
	size_t _ite;
	size_t _k;
	Timer _timer;
};

template<template<bool T> class LocalSearchT, bool isTraceOn> inline void Vns::run(
		size_t maxIte, size_t magMax) {
	_ite = 0;
	_globalIte = 0;
	LocalSearchT<false> ls;
	do {
		_k = 0;
		do {
			++_ite;
			++_globalIte;
			restart();
			shake(++_k);
			_input.ite() = 0;
			ls(_input);

			if (_best.second > 1e-6 + _input.cost()) {
				save();
				if (isTraceOn)
					out();
				_k = 0;
				_ite = 0;
			}
		} while (_k < magMax);
	} while (_ite < maxIte);
	if (isTraceOn)
		out();
}

#endif /* VNS_HPP_ */
