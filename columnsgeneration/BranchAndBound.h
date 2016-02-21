#ifndef BRANCH_AND_BOUND_HPP
#define BRANCH_AND_BOUND_HPP

#include "CliquePartitionProblem.h"
#include "gencol.h"
#include "IOracle.h"

#include "LpMaster.h"

class Node;
class BranchAndBound {
public:
	BranchAndBound(CliquePartitionProblem const &, AvailableOracle oracle =
			bMILP);
	virtual ~BranchAndBound();
public:
	CliquePartitionProblem const * _input;

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

	IMaster * _master;

	IOracle * _vnsGenerator;
	IOracle * _mipGenerator;

	double bestFeasible() const;
	double bestPossible() const;
private:
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
