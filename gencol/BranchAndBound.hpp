#ifndef BRANCH_AND_BOUND_HPP
#define BRANCH_AND_BOUND_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "LpMaster.hpp"
#include "VnsGenerator.hpp"
#include "MipGenerator.hpp"
class Node;
class BranchAndBound{
public:
	BranchAndBound(BipartiteGraph const &);
	~BranchAndBound();
public:
	BipartiteGraph const & _input;

	void columnGeneration(Node *);
	void branch(Node *, size_t , size_t );
	void run();
private:
	
	Double _rd;
	
	LpMaster _master;
	VnsGenerator _vnsGenerator;
	MipGenerator _mipGenerator;

	Node * _root;
	std::multimap<Double, Node * > _nodesByUpperBounds;

};

#endif 
