#ifndef COLUMN_HPP
#define COLUMN_HPP

#include "ClusteringProblem.h"
#include "Decision.h"
#include "gencol.h"

class Column {
public:
	Column(ClusteringProblem const &);
	Column(ClusteringProblem const *);
public:
	Double & cost();
	Double const & cost() const;
	Double & reducedCost();
	Double const & reducedCost() const;

	Double computeCost() const;
	Double computeReducedCost(DoubleVector const & dual) const;
	virtual ~Column();

	int size() const;
	int & id() const;
	bool check(DoubleVector const &) const;
	bool check() const;
	void print(std::ostream & = std::cout) const;

	bool insert(int v);
	void erase(int v);
	bool contains(int v) const;
	IntSet & v();
	IntSet const & v() const;

	bool operator<(Column const &) const;

	int violation(DecisionList const &) const;
	int violation(Decision const &) const;
	bool contains(int, int) const;
private:
	ClusteringProblem const * _input;
	Double _cost;
	Double _reducedCost;
	IntSet _v;
	mutable int _id;
};
#endif 

