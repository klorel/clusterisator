#ifndef BRANCH_AND_BOUND_HPP
#define BRANCH_AND_BOUND_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "LpMaster.hpp"
#include "VnsGenerator.hpp"
#include "MipGenerator.hpp"

class BranchAndBound{
public:
	BranchAndBound(BipartiteGraph const &);
	~BranchAndBound();
public:
	BipartiteGraph const & _input;

	void columnGeneration();

private:
	
	Double _rd;
	
	LpMaster _master;
	VnsGenerator _vnsGenerator;
	MipGenerator _mipGenerator;

};

#endif 
