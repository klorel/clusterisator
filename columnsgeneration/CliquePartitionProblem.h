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

class ILpSolver;
class IOracle;
class Node;

typedef std::vector<std::map<int, Double>> AdjencyGraph;

typedef std::vector<std::map<int, double> > AllLinks;

class CliquePartitionProblem: public ClusteringProblem {
public:
	enum AvailableOrable {
		MILP, MIQP
	};
public:
	virtual ~CliquePartitionProblem();
	virtual std::string name(int v) const;
	virtual void adjencyGraph(AdjencyGraph &) const;

	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights & result) const;

	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const;
	virtual IOracle * newVnsOracle(DoubleVector const * dual,
			DecisionList const * decision) const;
	virtual double cst() const;
	virtual void cps(std::string const &fileName, ILpSolver & solver) const;

	virtual void exportAmpl(std::string const &) const;

	IOracle * newMilpOracle() const;
	IOracle * newMiqpOracle() const;

	virtual void writeSolution(FractionnarySolution const&, double) const;
public:
	virtual Double computeCost(IntSet const &) const;
	virtual Double computeCost(IndexedList const &) const;

	virtual void update(int id, bool wasIn, DoubleVector &gradient) const;
	virtual void gradient(IndexedList const & v, DoubleVector &) const;
public:
	Edges & getEdges();
	Edges const & getEdges() const;

	std::map<int, double> const & allLinks(int v) const;

	std::vector<Edge> & getCosts();
	std::vector<Edge> const & getCosts() const;

	AllLinks & getAllLinks();
	AllLinks const & getAllLinks() const;

	virtual int nV() const;
	int &nV();
	void clear();

	virtual void cpCost(DoubleVector &result) const {};

public:
	int _n;
	// edges with cost
	Edges _edges2;

	AllLinks _allLinks;
	std::vector<Edge> _costs;
};

inline void CliquePartitionProblem::exportAmpl(
		std::string const & fileName) const {
	std::ofstream file(fileName.c_str());
	for (auto const & edge : getEdges()) {
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

inline CliquePartitionProblem::~CliquePartitionProblem() {

}

inline void CliquePartitionProblem::adjencyGraph(AdjencyGraph & result) const {
	result.assign(nV(), AdjencyGraph::value_type());
	for (auto & s : result)
		s.clear();
	for (auto const & e : getEdges()) {
		result[e._i][e._j] += e._v;
		result[e._j][e._i] += e._v;
	}

}
inline std::string CliquePartitionProblem::name(int v) const {
	return GetStr("Y_", v);
}

#endif /* ICLIQUEPARTITIONNINPROBLEM_H_ */
