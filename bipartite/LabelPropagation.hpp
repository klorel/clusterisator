/*
 * LabelPropagation.hpp
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef LABELPROPAGATION_HPP_
#define LABELPROPAGATION_HPP_

#include "gencol.h"

#include "BipartiteGraph.hpp"
#include "ModularityBPartition.hpp"

class LabelPropagation {
public:
	LabelPropagation(BipartiteGraph const &, ModularityBPartition &);
	virtual ~LabelPropagation();

	bool operator()(size_t);
	bool operator()();
	Double w(size_t id, size_t neighbor) const;
	Double k(size_t id) const;
	Double deltaK(size_t, size_t) const;
private:
	void buildGraph();
private:
	BipartiteGraph const * _input;
	ModularityBPartition * _solution;
	std::vector<IntSet> _graph;
};
inline Double LabelPropagation::w(size_t id, size_t neighbor) const {
	if (id < _input->nR()) {
		assert(neighbor >= _input->nR());
		return _input->w(id, neighbor - _input->nR());
	} else {
		assert(neighbor < _input->nR());
		return _input->w(neighbor, id - _input->nR());
	}
}
inline Double LabelPropagation::k(size_t id) const {
	if (id < _input->nR())
		return _input->kR(id);
	else
		return _input->kB(id - _input->nR());
}
#endif /* LABELPROPAGATION_HPP_ */
