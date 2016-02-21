/*
 * ICliquePartitionninProblem.h
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#ifndef CLIQUE_PARTITION_PROBLEM_H_
#define CLIQUE_PARTITION_PROBLEM_H_

#include "common.h"
#include "Edge.h"
#include "gencol.h"
#include "ClusteringProblem.h"

class IOracle;
class Node;

typedef std::vector<std::map<size_t, Double>> AdjencyGraph;

class CliquePartitionProblem: public ClusteringProblem {
public:
	enum AvailableOrable {
		MILP, MIQP
	};
public:
	virtual Edges const & edges() const = 0;
	virtual std::vector<Edge> const & costs() const = 0;

public:
	virtual ~CliquePartitionProblem();
	virtual std::string problemName() const;
	virtual std::string name(size_t v) const;
	virtual void adjencyGraph(AdjencyGraph &) const;

	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights & result) const;

	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const;
	virtual IOracle * newVnsOracle(DoubleVector const * dual,
			DecisionList const * decision) const;
	virtual double cst() const;
	virtual void cps(std::string const &fileName) const;

	virtual void cpCost(DoubleVector &) const = 0;

	virtual void exportAmpl(std::string const &) const;
public:
//	Edges const * const _edges;
//	DoubleVector const * const _denseCost;

};

inline void CliquePartitionProblem::exportAmpl(
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

inline double CliquePartitionProblem::cst() const {
	return 0;
}
inline std::string CliquePartitionProblem::problemName() const {
	return "";
}

inline CliquePartitionProblem::~CliquePartitionProblem() {

}

inline void CliquePartitionProblem::adjencyGraph(AdjencyGraph & result) const {
	result.assign(nV(), AdjencyGraph::value_type());
	for (auto & s : result)
		s.clear();
	for (auto const & e : edges()) {
		result[e._i][e._j] += e._v;
		result[e._j][e._i] += e._v;
	}

}
inline std::string CliquePartitionProblem::name(size_t v) const {
	return GetStr("Y_", v);
}

#endif /* ICLIQUEPARTITIONNINPROBLEM_H_ */
