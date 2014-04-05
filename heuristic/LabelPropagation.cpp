/*
 * LabelPropagation.cpp
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#include "LabelPropagation.hpp"

LabelPropagation::LabelPropagation(BipartiteGraph const & input,
		ModularityBPartition & solution) :
		_input(&input), _solution(&solution) {
	buildGraph();
}

LabelPropagation::~LabelPropagation() {

}

bool LabelPropagation::operator()(size_t id) {
	std::map<size_t, Double> links;
	for (size_t r(0); r < _input->nR() && id >= _input->nR(); ++r) {
		links[_solution->label(r)] += w(id, r);
	}
	for (size_t b(0); b < _input->nB() && id < _input->nR(); ++b) {
		links[_solution->label(_input->nR() + b)] += w(id, _input->nR() + b);
	}
	Double const loss(links[_solution->label(id)]);
	std::multimap<Double, size_t, std::greater<Double>> delta;
	for (auto const & n : links) {
		delta.insert(std::make_pair(n.second - loss, n.first));
	}
	bool success(delta.begin()->first > 1e-6);
	if (success) {
		_solution->checkScore();
		_solution->shift(id, delta.begin()->second);
		_solution->score() += delta.begin()->first;
		_solution->checkScore();
	}
	return success;
}
bool LabelPropagation::operator()() {
	bool success(false);
	bool stop(true);
	_solution->checkScore();
	do {
		stop = true;
		for (size_t id(0); id < _input->nV(); ++id) {
			if (operator ()(id)) {
				stop = false;
				success = true;
			}
		}
	} while (!stop);
	_solution->checkScore();
	return success;
}

void LabelPropagation::buildGraph() {
	_graph.clear();
	_graph.resize(_input->nV(), IntSet());
	for (Edge const & e : _input->edges()) {
		_graph[e._i].insert(_input->nR() + e._j);
		_graph[_input->nR() + e._j].insert(e._i);
	}
}