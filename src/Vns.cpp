/*
 * Vns.cpp
 *
 *  Created on: 20 août 2012
 *      Author: manuel
 */

#include "Vns.hpp"

Vns::Vns(KMAlgo & algo) :
		_algo(algo), _best(algo.partition(), algo.computeCost()), _nodes(
				_best.first.nbObs()) {
	_ite = 0;
	for (size_t i(0); i < _nodes.size(); ++i)
		_nodes[i] = i;
}

Vns::~Vns() {

}

void Vns::restart() {
	_algo.partition().set(_best.first);
}

void Vns::save() {
	_best.first.set(_algo.partition());
	_best.second = _algo.cost();
	_k = 0;
}
void Vns::shake(size_t mag) {
	size_t n(_nodes.size());
	for (size_t i(0); i < mag && n > 0; ++i) {
		size_t ite(0);
		size_t const obs(PopRandom(_nodes, n));
		bool success(false);
		do {
			++ite;
			size_t const k(Number::Generator() % _algo.partition().getK());
			if (k != _algo.partition().label(obs)) {
				_algo.shift(i, k);
				success = true;
			}
			if (ite > 10 * _algo.partition().getK())
				std::cout << "ite > 10 * _algo.partition().getK())\n";
		} while (!success);
	}
}

void Vns::run(size_t maxIte, size_t magMax) {
	_ite = 0;
	_algo.isTraceOn() = false;
	do {
		_k = 0;
		do {
			++_ite;
			restart();
			shake(++_k);
			_algo.hMeans(0);
			_algo.kMeans(0);
			out();
			if (_best.second > 1e-10 + _algo.cost()) {
				save();
				out();
//				assert(false);
			}
		} while (_k < magMax);
	} while (_ite < maxIte);
}

void Vns::out() const {
	OUT<< "----------------------------------------\n";
	OUT<< std::setw(6)<<_timer.elapsed();
	OUT<< std::setw(6)<<_ite;
	OUT<< std::setw(6)<<_k;
	OUT<< std::setw(25)<<_best.second;
	OUT<< std::setw(25)<<_algo.cost();
	OUT<<"\n";
	OUT<< "----------------------------------------\n";
}
