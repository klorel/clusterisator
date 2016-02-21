#include "ModularityBPartition.h"

ModularityBPartition::ModularityBPartition(BipartiteGraph const & input,
		size_t k) :
		Partition(input.nV(), k), _input(&input) {
	DoubleVector w(input.nV());
	for (size_t r(0); r < input.nR(); ++r)
		w[r] = input.k(r);
	for (size_t b(0); b < input.nB(); ++b)
		w[b] = _input->k(_input->nR() + b);
	setWeights(w);
	_score = 0;
}
ModularityBPartition::ModularityBPartition(BipartiteGraph const & input,
		Partition const &rhs) :
		Partition(rhs), _input(&input) {
	init();
	_score = 0;
}
ModularityBPartition::ModularityBPartition(BipartiteGraph const * input,
		size_t k) :
		Partition(input->nV(), k), _input(input) {
	init();
	_score = 0;
}
ModularityBPartition::ModularityBPartition(BipartiteGraph const * input,
		Partition const &rhs) :
		Partition(rhs), _input(input) {
	init();
	_score = 0;
}
ModularityBPartition::~ModularityBPartition() {

}

Double ModularityBPartition::computeScore() const {
	Double result(0);
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			if (label(r) == label(_input->nR() + b)) {
				result += _input->w(r, b);
			}
		}
	}
	return result;
}
Double &ModularityBPartition::score() {
	return _score;
}
Double ModularityBPartition::score() const {
	return _score;
}

void ModularityBPartition::checkScore() const {
	if (std::fabs(_score - computeScore()) > 1e-6) {
		std::cout << std::setw(20) << _score;
		std::cout << std::setw(20) << computeScore();
		std::cout << std::setw(20) << _score - computeScore();
		std::cout << std::endl;
		assert(false && "WRONG SCORE");
	}

}
void ModularityBPartition::init() {
	DoubleVector w(_input->nV());
	for (size_t r(0); r < _input->nR(); ++r)
		w[r] = _input->k(r);
	for (size_t b(0); b < _input->nB(); ++b)
		w[b] = _input->k(_input->nR() + b);
	setWeights(w);
	oneLabel(_input->nV(), _input->nV());
}
Double ModularityBPartition::score(size_t k) const {
	Double result(0);
	for (Edge const & edge : _input->edges()) {
		if (label(edge._i) == k && label(_input->nR() + edge._j) == k)
			++result;
	}
	result *= _input->inv_m();
	Double Rc(0);
	for (size_t r(0); r < _input->nR(); ++r)
		if (label(r) == k)
			Rc += _input->k(r);
	Double Bc(0);
	for (size_t b(0); b < _input->nB(); ++b)
		if (label(_input->nR() + b) == k)
			Bc += _input->k(_input->nR() + b);
	return result - Rc * Bc * _input->inv_m() * _input->inv_m();
}
Double ModularityBPartition::scoreIfSwap(size_t k, size_t node) const {
	Double result(0);
	for (Edge const & edge : _input->edges()) {
		if (edge._i == node && label(_input->nR() + edge._j) == k
				&& label(edge._i) != k) {
			++result;
		} else if (_input->nR() + edge._j == node && label(edge._i) == k
				&& label(_input->nR() + edge._j) != k) {
			++result;
		} else if (label(edge._i) == k && label(_input->nR() + edge._j) == k)
			++result;
	}
	result *= _input->inv_m();
	Double Rc(0);
	for (size_t r(0); r < _input->nR(); ++r) {
		if (r == node && label(r) != k)
			Rc += _input->k(r);
		else if (label(r) == k)
			Rc += _input->k(r);
	}
	Double Bc(0);
	for (size_t b(0); b < _input->nB(); ++b)
		if (label(_input->nR() + b) == k && label(_input->nR() + b) != k)
			Bc += _input->k(_input->nR() + b);
		else if (label(_input->nR() + b) == k)
			Bc += _input->k(_input->nR() + b);
	return result - Rc * Bc * _input->inv_m() * _input->inv_m();
}
