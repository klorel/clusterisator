#ifndef BRANCH_AND_BOUND_HPP
#define BRANCH_AND_BOUND_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "LpMaster.hpp"
#include "VnsGenerator.hpp"
#include "MipGenerator.hpp"
#include "ModularityBPartition.hpp"

class Node;
class BranchAndBound {
public:
	BranchAndBound(BipartiteGraph const *);
	BranchAndBound(BipartiteGraph const &);
	~BranchAndBound();
public:
	BipartiteGraph const * _input;

	void columnGeneration();
	void branch(Node *, size_t, size_t);
	void init();
	void run();
	void treat(Node * node);
	void printTree(std::ostream & = std::cout) const;
	std::ostream & output();
	LpMaster & master();
	LpMaster const & master() const;

	void writeSolution()const;
private:

	Double _rd;
	DecisionList _decision;

	LpMaster _master;
	VnsGenerator _vnsGenerator;
	MipGenerator _mipGenerator;

	Node * _root;
	Node * _current;
	std::ostream * _output;
	std::multimap<Double, Node *, std::greater<Double>> _nodesByUpperBounds;

	Double _bestFeasible;
	Double _bestPossible;
	FractionnarySolution _solution;

};

#endif 
