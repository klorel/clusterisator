/*
 * LabelPropagation.hpp
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef LABELPROPAGATION_HPP_
#define LABELPROPAGATION_HPP_

#include "common.h"

#include "BipartiteGraph.hpp"
#include "ModularityBPartition.hpp"

#include "ICliquePartition.h"
#include "ICliquePartitionProblem.h"

class LabelPropagation2 {
public:
	LabelPropagation2(ICliquePartition &solution);
	virtual ~LabelPropagation2();

	bool operator()(size_t);
	bool operator()();
private:
	void buildGraph();
private:
	ICliquePartitionProblem const & _input;
	ICliquePartition & _solution;
	AdjencyGraph _graph;
};
#endif /* LABELPROPAGATION_HPP_ */
