/*
 * ClusteringProblem.h
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#ifndef I_CLUSTERING_PROBLEM_H_
#define I_CLUSTERING_PROBLEM_H_

#include "common.h"
#include "Edge.h"
#include "gencol.h"
class IOracle;
class Node;

typedef std::vector<std::map<size_t, Double>> AdjencyGraph;

class ClusteringProblem {
public:
	virtual size_t nV() const =0;
	virtual void writeSolution(FractionnarySolution const&, double) const = 0;
	virtual void update(size_t id, bool wasIn, DoubleVector &gradient) const= 0;
	virtual void gradient(IndexedList const & v, DoubleVector &) const= 0;
	virtual Double computeCost(std::set<size_t> const &) const = 0;
	virtual Double computeCost(IndexedList const &) const = 0;
	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights & result) const = 0;
public:
	virtual void branchingSelection(Node const & node, size_t &noeud1,
			size_t &noeud2) const;

	virtual std::pair<size_t, size_t> branchingSelection(
			DecisionSet const & decisions, BranchingWeights & weights) const;

	virtual bool checkGradient(IndexedList const & v,
			DoubleVector const & g) const;

	virtual IOracle * getExactOracle() const;

	virtual IOracle * getVnsOracle() const;

	virtual std::string problemName() const;

public:
	void setExactOracle(IOracle *);
	void setVnsOracle(IOracle *);
public:
	ClusteringProblem();
	virtual ~ClusteringProblem();
private:
	IOracle * _exactOracle;
	IOracle * _vnsOracle;
};

#endif /* ICLIQUEPARTITIONNINPROBLEM_H_ */
