#ifndef BRANCH_AND_BOUND_HPP
#define BRANCH_AND_BOUND_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "LpMaster.hpp"

#include "IOracle.h"
#include "ModularityBPartition.hpp"

enum AvailableOracle {
	MILP, MIQP, bMILP
};
class Node;
class BranchAndBound {
public:
	BranchAndBound(BipartiteGraph const &, AvailableOracle oracle = bMILP);
	virtual ~BranchAndBound();
public:
	BipartiteGraph const * _input;

	void columnGeneration();

	void init();
	void run();
	void treat(Node * node);
	void printTree(std::ostream & = std::cout) const;
	std::ostream & output();
	void setOutput(std::ostream & = std::cout);
	IMaster & master();
	IMaster const & master() const;

	void writeSolution() const;
private:
	IMaster * _master;

	IOracle * _vnsGenerator;
	IOracle * _mipGenerator;

	Node * _root;
	Node * _current;

	DecisionList _decision;

	std::ostream * _output;
	std::multimap<Double, Node *, std::greater<Double>> _nodesByUpperBounds;

	Double _bestFeasible;
	Double _bestPossible;
	FractionnarySolution _bestSolution;

};

#endif 
