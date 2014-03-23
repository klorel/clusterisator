#ifndef COLUMN_HPP
#define COLUMN_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "Decision.hpp"

class Column {
public:
	Double & cost();
	Double const & cost() const;
	Double & reducedCost();
	Double const & reducedCost() const;

	Double computeCost() const;
	Double computeReducedCost(DoubleVector const & dual) const;
	Column(BipartiteGraph const &);
	Column(BipartiteGraph const *);
	virtual ~Column();
	void addElement(size_t);
	void delElement(size_t);
	size_t size() const;
	size_t & id() const;
	void check(DoubleVector const &) const;
	void print(std::ostream & = std::cout) const;

	std::set<size_t> & r();
	std::set<size_t> const & r() const;

	std::set<size_t> & b();
	std::set<size_t> const & b() const;

	bool operator<(Column const &) const;

	size_t violation(DecisionList const &) const;
	size_t violation(Decision const &) const;
	bool constains(size_t, size_t) const;
	bool constainsR(size_t) const;
	bool constainsB(size_t) const;
private:
	BipartiteGraph const * _input;
	Double _cost;
	Double _reducedCost;
	std::set<size_t> _r;
	std::set<size_t> _b;
	mutable size_t _id;
};

#endif 

