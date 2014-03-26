#ifndef COLUMN_HPP
#define COLUMN_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "Decision.hpp"

class Column {
public:
	Column(BipartiteGraph const &);
	Column(BipartiteGraph const *);
public:
	Double & cost();
	Double const & cost() const;
	Double & reducedCost();
	Double const & reducedCost() const;

	Double computeCost() const;
	Double computeReducedCost(DoubleVector const & dual) const;
	virtual ~Column();

	size_t size() const;
	size_t & id() const;
	bool check(DoubleVector const &) const;
	bool check() const;
	void print(std::ostream & = std::cout) const;

	bool insert(size_t v);
	void erase(size_t v);
	bool contains(size_t v) const;
	std::set<size_t> & v();
	std::set<size_t> const & v() const;

//	std::set<size_t> & r();
//	std::set<size_t> const & r() const;
//
//	std::set<size_t> & b();
//	std::set<size_t> const & b() const;

	bool operator<(Column const &) const;

	size_t violation(DecisionList const &) const;
	size_t violation(Decision const &) const;
	bool contains(size_t, size_t) const;
private:
	BipartiteGraph const * _input;
	Double _cost;
	Double _reducedCost;
//	std::set<size_t> _r;
//	std::set<size_t> _b;
	std::set<size_t> _v;
	mutable size_t _id;
};

typedef std::set<Column> Columns;
#endif 

