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
	virtual Edges const & edges() const = 0;
public:
	virtual void writeSolution(FractionnarySolution const&, double) const = 0;

	virtual Double computeCost(std::set<size_t> const &) const = 0;
	virtual Double computeCost(IndexedList const &) const = 0;

	virtual void update(size_t id, bool wasIn, DoubleVector &gradient) const= 0;
	virtual void gradient(IndexedList const & v, DoubleVector &) const= 0;
	virtual std::vector<Edge> const & costs() const = 0;

public:
	virtual ~ICliquePartitionProblem();
	virtual std::string problemName() const;
	virtual std::string name(size_t v) const;
	virtual void adjencyGraph(AdjencyGraph &) const;
	virtual bool checkGradient(IndexedList const & v,
			DoubleVector const & g) const;

	virtual void branchingSelection(Node const & node, size_t &noeud1,
			size_t &noeud2) const;
	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights & result) const;

	virtual std::pair<size_t, size_t> branchingSelection(
			DecisionSet const & decisions, BranchingWeights & weights) const;

	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const;
	virtual IOracle * newVnsOracle(DoubleVector const * dual,
			DecisionList const * decision) const;
	virtual double cst() const;
	virtual void cps(std::string const &fileName) const;

	virtual void cpCost(DoubleVector &) const = 0;

	virtual void exportAmpl(std::string const &) const;

};

inline void ICliquePartitionProblem::exportAmpl(
		std::string const & fileName) const {
	std::ofstream file(fileName.c_str());
	for (auto const & edge : edges()) {
		file << std::setw(6) << 1 + edge._i;
		file << std::setw(6) << 1 + edge._j;
		file << std::endl;
	}
	file << ";" << std::endl;
	file.close();
}

inline double ICliquePartitionProblem::cst() const {
	return 0;
}
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
