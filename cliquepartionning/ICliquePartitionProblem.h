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
class ICliquePartitionProblem {
public:
	virtual ~ICliquePartitionProblem();

	virtual size_t nV() const =0;

	virtual Edges const & edges() const =0;

	virtual Edge const & edge(size_t i, size_t j) const =0;

	virtual double score(size_t i, size_t j) const =0;

	virtual double k(size_t i) const=0;

	virtual void adjencyGraph(std::vector<IntSet> &) const;

	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const=0;

	virtual void branchingSelection(Node const & node, size_t &noeud1,
			size_t &noeud2) const = 0;
	virtual void writeSolution(FractionnarySolution const&, double) const = 0;
};

#endif /* ICLIQUEPARTITIONNINPROBLEM_H_ */
