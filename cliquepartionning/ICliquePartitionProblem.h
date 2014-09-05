/*
 * ICliquePartitionninProblem.h
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#ifndef I_CLIQUE_PARTITION_PROBLEM_H_
#define I_CLIQUE_PARTITION_PROBLEM_H_

#include "common.h"
#include "Edge.h"
#include "gencol.h"
class IOracle;
class Node;

typedef std::vector<std::map<size_t, Double>> AdjencyGraph;

class ICliquePartitionProblem {
public:
	virtual ~ICliquePartitionProblem();

	virtual size_t nV() const =0;

	virtual Edges const & edges() const =0;

	virtual double k(size_t i) const=0;

	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const=0;

	virtual void branchingSelection(Node const & node, size_t &noeud1,
			size_t &noeud2) const = 0;
	virtual void writeSolution(FractionnarySolution const&, double) const = 0;
public:
	virtual void adjencyGraph(AdjencyGraph &) const;
};

inline ICliquePartitionProblem::~ICliquePartitionProblem() {

}

inline void ICliquePartitionProblem::adjencyGraph(AdjencyGraph & result) const {
	result.assign(nV(), AdjencyGraph::value_type());
	for (auto & s : result)
		s.clear();
	for (auto const & e : edges()) {
		result[e._i][e._j] += e._v;
		result[e._j][e._i] += e._v;
	}

}

#endif /* ICLIQUEPARTITIONNINPROBLEM_H_ */
