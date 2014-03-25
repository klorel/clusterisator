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
	virtual ~VnsGeneratorSolution();
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
	Double gradient(size_t) const;
	Double deltaCost(size_t) const;
	Double deltaDual(size_t) const;
public:
	static bool IsBetter(VnsGeneratorSolution const & p,
			VnsGeneratorSolution const & q, DecisionList const & decisions);
	Double dual(size_t) const;
private:
	BipartiteGraph const * _input;
	DoubleVector const * _dual;
public:
	IndexedList _v;

	Double _cost;
	Double _reducedCost;
};

inline Double VnsGeneratorSolution::dual(size_t n) const {
	return (*_dual)[n];
}
inline Double VnsGeneratorSolution::gradient(size_t v) const {
	Double result(0);
	for (auto const & link : _input->allLinks(v))
		if (_v.contains(link.first))
			result += link.second;
	return result;
}
inline Double VnsGeneratorSolution::deltaCost(size_t id) const {
	if (_v.contains(id))
		return -gradient(id);
	else
		return gradient(id);
//	if (id < _input->nR())
//		return deltaCostR(id);
//	else
//		return deltaCostB(id - _input->nR());
}
inline Double VnsGeneratorSolution::deltaDual(size_t id) const {
	if (_v.contains(id))
		return -dual(id);
	else
		return +dual(id);

}

#endif /* GRAPH_HPP */
