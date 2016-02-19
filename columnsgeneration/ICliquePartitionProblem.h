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
	virtual size_t nV() const =0;

	virtual Edges const & edges() const =0;

	virtual double k(size_t i) const=0;

	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const;
	virtual IOracle * newVnsOracle(DoubleVector const * dual,
			DecisionList const * decision) const;
public:

	virtual void branchingSelection(Node const & node, size_t &noeud1,
			size_t &noeud2) const;
	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights & result) const;

	virtual std::pair<size_t, size_t> branchingSelection(DecisionSet const & decisions,
			BranchingWeights & weights) const;
public:
	virtual void writeSolution(FractionnarySolution const&, double) const = 0;

	virtual Double computeCost(std::set<size_t> const &) const = 0;
	virtual Double computeCost(IndexedList const &) const = 0;

	virtual Double gradient(size_t id, IndexedList const & v) const = 0;
	virtual void update(size_t id, bool wasIn, DoubleVector &gradient) const= 0;
	virtual void gradient(IndexedList const & v, DoubleVector &) const= 0;
	virtual std::vector<Edge> const & costs() const = 0;
public:
	virtual ~ICliquePartitionProblem();
	virtual std::string problemName() const;
	virtual std::string name(size_t v) const;
	virtual void adjencyGraph(AdjencyGraph &) const;
};
inline std::string ICliquePartitionProblem::problemName() const {
	return "";
}

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
inline std::string ICliquePartitionProblem::name(size_t v) const {
	return GetStr("Y_", v);
}

#endif /* ICLIQUEPARTITIONNINPROBLEM_H_ */
