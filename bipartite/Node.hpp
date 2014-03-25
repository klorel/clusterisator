#ifndef NODE_H
#define NODE_H

#include "gencol.h"
#include "BranchAndBound.hpp"
#include "Decision.hpp"

class Node {
public:
public:
	bool isRoot() const;
	Node(BranchAndBound &);
	Node(BranchAndBound *);
	Node(Node const * father, bool cannot, size_t noeud1, size_t noeud2);
	virtual ~Node();
	Double lb() const;
	Double ub() const;
	Double &lb();
	Double &ub();
	size_t id() const;
	size_t & id();
	FractionnarySolution & lbSolution();
	bool &isInteger();
	bool isInteger() const;
	Node const * father() const;
//	bool cannot() const;
//	size_t r() const;
//	size_t b() const;

	void decisions(DecisionList&) const;
	void decisions(DecisionSet&) const;
	Decision const & decision() const;
private:
	BranchAndBound * _branchAndBound;
	bool _isInteger;
	FractionnarySolution _lbSolution;

	Node const * _father;
	Double _ub;
	Double _lb;

	Decision _decision;

	size_t _id;
};

class LbPredicate {
public:
	bool operator()(Node const * p, Node const * q) const {
		return p->lb() < q->lb();
	}
	bool operator()(Node const & p, Node const & q) const {
		return p.lb() < q.lb();
	}
};

class UbPredicate {
public:
	bool operator()(Node * p, Node * q) const {
		return p->ub() > q->ub();
	}
	bool operator()(Node const * p, Node const * q) const {
		return p->ub() > q->ub();
	}
	bool operator()(Node const & p, Node const & q) const {
		return p.ub() > q.ub();
	}
};

#endif /* CG_MASTER_HPP */
