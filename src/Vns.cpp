/*
 * Vns.cpp
 *
 *  Created on: 20 août 2012
 *      Author: manuel
 */

#include "Vns.hpp"

Vns::Vns(KMInput & input) :
		_input(input), _best(input, input.computeCost()), _nodes(
				_best.first.nbObs()) {
	_ite = 0;
	_globalIte = 0;
	_k = 0;
	for (size_t i(0); i < _nodes.size(); ++i)
		_nodes[i] = i;
}

Vns::~Vns() {

}

void Vns::restart() {
	_input.set(_best.first);
}

void Vns::save() {
	_best.first.set(_input);
	_best.second = _input.cost();

}
void Vns::shake(size_t mag) {
	size_t n(_nodes.size());
	for (size_t i(0); i < mag && n > 0; ++i) {
		size_t ite(0);
		size_t const obs(PopRandom(_nodes, n));
		bool success(false);
		do {
			++ite;
			size_t const k(Number::Generator() % _input.getK());
			if (k != _input.label(obs)) {
				_input.shift(i, k);
				success = true;
			}
			if (ite > 10 * _input.getK())
				std::cout << "ite > 10 * _input.getK())\n";
		} while (!success);
	}
}

void Vns::out() const {
	OUT<< std::setw(10)<<_timer.elapsed();
	OUT<< std::setw(10)<<_globalIte;
	OUT<< std::setw(10)<<_ite;
	OUT<< std::setw(10)<<_k;
	OUT<< std::setw(25)<<std::setprecision(8)<< _best.second;
	OUT<<"\n";
}
