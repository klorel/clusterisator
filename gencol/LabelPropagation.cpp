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
//	MY_PRINT(id);
	for (size_t r(0); r < _input->nR() && id >= _input->nR(); ++r) {
		links[_solution->label(r)] += w(id, r);
	}
	for (size_t b(0); b < _input->nB() && id < _input->nR(); ++b) {
		links[_solution->label(_input->nR() + b)] += w(id, _input->nR() + b);
	}
//	for (auto const & n : _graph[id]) {
//		links[_solution->label(n)] += w(id, n);
//	}
	Double const loss(links[_solution->label(id)]);
	std::multimap<Double, size_t, std::greater<Double>> delta;
//	std::cout << "loss : " << loss << std::endl;
//	if (!_solution->allLabelsUsed())
//		delta.insert(std::make_pair(-loss, _solution->getUnUsedLabel()));
	for (auto const & n : links) {
//		std::cout << n.first << " | " << n.second << " | " << loss << std::endl;
		delta.insert(std::make_pair(n.second - loss, n.first));
	}
//	std::cout << delta.begin()->first << std::endl;
	bool success(delta.begin()->first > 1e-6);
	if (success) {
		_solution->checkScore();
		_solution->shift(id, delta.begin()->second);
		_solution->score() += delta.begin()->first;
//		std::cout << std::setw(25) << loss;
//		std::cout << std::setw(25) << links[delta.begin()->second];
//		std::cout << std::endl;
		_solution->checkScore();
//		assert(
//				std::fabs(delta.begin()->first + old - _solution->score())
//						< 1e-10);
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
//				std::cout << std::setw(4) << id;
//				std::cout << std::setw(4) << _solution->nbLabels();
//				std::cout << std::setw(25) << _solution->score();
//				std::cout << std::endl;
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
