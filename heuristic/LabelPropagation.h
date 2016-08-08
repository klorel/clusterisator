/*
 * LabelPropagation.h
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef LABELPROPAGATION_HPP_
#define LABELPROPAGATION_HPP_

#include "common.h"

#include "BipartiteGraph.h"
#include "ModularityBPartition.h"

namespace bipartite {
class LabelPropagation {
public:
	LabelPropagation(BipartiteGraph const &, ModularityBPartition &);
	virtual ~LabelPropagation();

	bool operator()(int);
	bool operator()();
	Double w(int id, int neighbor) const;
	Double k(int id) const;
private:
	void buildGraph();
private:
	BipartiteGraph const * _input;
	ModularityBPartition * _solution;
	std::vector<IntSet> _graph;
};
inline Double LabelPropagation::w(int id, int neighbor) const {
	if (id < _input->nR()) {
		assert(neighbor >= _input->nR());
		return _input->w(id, neighbor - _input->nR());
	} else {
		assert(neighbor < _input->nR());
		return _input->w(neighbor, id - _input->nR());
	}
}
inline Double LabelPropagation::k(int id) const {
	return _input->k(id);
}
}
#endif /* LABELPROPAGATION_HPP_ */
