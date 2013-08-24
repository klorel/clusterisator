#ifndef VNS_GENERATOR_SOLUTION_HPP
#define VNS_GENERATOR_SOLUTION_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "Column.hpp"
#include "IndexedList.hpp"
#include "Decision.hpp"

class Node;
class VnsGeneratorSolution {
public:
	VnsGeneratorSolution(BipartiteGraph const &, DoubleVector const & dual);
	VnsGeneratorSolution(BipartiteGraph const *, DoubleVector const * dual);
	~VnsGeneratorSolution();
	VnsGeneratorSolution & operator=(VnsGeneratorSolution const &);
public:
	void swap(size_t id);
	void swap(size_t id, Double, Double);

	Double computeCost() const;
	Double computeReducedCost() const;
	void build(Column &);
	size_t violation(DecisionList const &) const;
	void check() const;
	void clear();
	Double gradientR(size_t) const;
	Double gradientB(size_t) const;
	Double deltaCostR(size_t) const;
	Double deltaCostB(size_t) const;
	Double deltaCost(size_t) const;
	Double deltaDual(size_t) const;
public:
	static bool IsBetter(VnsGeneratorSolution const & p,
			VnsGeneratorSolution const & q, DecisionList const & decisions);
	Double dual(size_t) const;
	Double dualR(size_t) const;
	Double dualB(size_t) const;
private:
	BipartiteGraph const * _input;
	DoubleVector const * _dual;
public:
	IndexedList _v;
	IndexedList _r;
	IndexedList _b;

	Double _cost;
	Double _reducedCost;
};


inline Double VnsGeneratorSolution::dual(size_t n) const {
	return (*_dual)[n];
}
inline Double VnsGeneratorSolution::dualR(size_t r) const {
	return dual(r);

}
inline Double VnsGeneratorSolution::dualB(size_t b) const {
	return dual(_input->nR() + b);

}
inline Double VnsGeneratorSolution::gradientR(size_t r) const {
	Double result(0);
	for (auto const & b : _b)
		result += _input->w(r, b);
	return result;
}
inline Double VnsGeneratorSolution::gradientB(size_t b) const {
	Double result(0);
	for (auto const & r : _r)
		result += _input->w(r, b);
	return result;
}
inline Double VnsGeneratorSolution::deltaCostR(size_t r) const {
	if (_r.contains(r))
		return -gradientR(r);
	else
		return gradientR(r);

}
inline Double VnsGeneratorSolution::deltaCostB(size_t b) const {
	if (_b.contains(b))
		return -gradientB(b);
	else
		return gradientB(b);

}
inline Double VnsGeneratorSolution::deltaCost(size_t id) const {
	if (id < _input->nR())
		return deltaCostR(id);
	else
		return deltaCostB(id - _input->nR());
}
inline Double VnsGeneratorSolution::deltaDual(size_t id) const {
	if (_v.contains(id))
		return -dual(id);
	else
		return +dual(id);

}

#endif /* GRAPH_HPP */
