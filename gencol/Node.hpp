#ifndef NODE_H
#define NODE_H

#include "gencol.h"
#include "BranchAndBound.hpp"


class Node{
public:
public:
	Node(BranchAndBound & );
	Node(Node const * father );
	~Node();
	Double lb()const;
	Double ub()const;
	Double &lb();
	Double &ub();

	FractionnarySolution & lbSolution();

	Node const * father()const;
	bool cannot()const;
	size_t r()const;
	size_t b()const;
private:
	BranchAndBound & _branchAndBound;
	FractionnarySolution _lbSolution;

	Node const * _father;
	Double _ub;
	Double _lb;

	bool _cannot;
	size_t _r;
	size_t _b;

};

class LbPredicate{
public:
	bool operator()(Node const * p, Node const * q)const{
		return p->lb() < q->lb();
	}
	bool operator()(Node const & p, Node const & q)const{
		return p.lb() < q.lb();
	}
};

#endif /* CG_MASTER_HPP */
