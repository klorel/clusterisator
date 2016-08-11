#ifndef NODE_H
#define NODE_H

#include "BranchAndBound.h"
#include "Decision.h"
#include "gencol.h"

class Node {
 public:
 public:
  bool isRoot() const;
  Node(BranchAndBound const&);
  Node(Node const * father, bool cannot, int noeud1, int noeud2);
  virtual ~Node();
 public:
  Double lb() const;
  Double ub() const;
  Double &lb();
  Double &ub();
  int id() const;
  int & id();
  FractionnarySolution & lbSolution();
  FractionnarySolution const & lbSolution() const;
  bool &isInteger();
  bool isInteger() const;
  Node const * father() const;
  void decisions(DecisionList&) const;
  void decisions(DecisionSet&) const;
  Decision const & decision() const;
 private:
  BranchAndBound const * _branchAndBound;
  bool _isInteger;
  FractionnarySolution _lbSolution;

  Node const * _father;
  Double _ub;
  Double _lb;

  Decision _decision;

  int _id;
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
