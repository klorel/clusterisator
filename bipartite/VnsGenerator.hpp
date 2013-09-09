#ifndef VNS_GENERATOR_HPP
#define VNS_GENERATOR_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "Column.hpp"
#include "IndexedList.hpp"
#include "VnsGeneratorSolution.hpp"
#include "Decision.hpp"

class Node;
class VnsGenerator {
public:
	VnsGenerator(BipartiteGraph const & input, DoubleVector const & dual,
			DecisionList const &);
	VnsGenerator(BipartiteGraph const * input, DoubleVector const * dual,
			DecisionList const *);
	~VnsGenerator();

	void shake(size_t k);
	void compute();
	bool localSearch();
	bool run(size_t iteMax, bool stopAtFirst);
	std::set<Column> const & columns() const;
	void sortedColumns(
			std::multimap<Double, Column const *, std::greater<Double>> &) const;

	Double dual(size_t) const;
	Double dualR(size_t) const;
	Double dualB(size_t) const;
	Double deltaR(size_t) const;
	Double deltaB(size_t) const;

	void initialize();

	size_t violationIfR(size_t r) const;
	size_t violationIfB(size_t b) const;
private:

	BipartiteGraph const * _input;
	DoubleVector const * _dual;

	IndexedList _allNodes;

	std::set<Column> _columns;

	VnsGeneratorSolution _current;
	VnsGeneratorSolution _best;

	DecisionList const * _decisions;
};

inline Double VnsGenerator::dual(size_t n) const {
	return (*_dual)[n];
}
inline Double VnsGenerator::dualR(size_t r) const {
	return dual(r);

}
inline Double VnsGenerator::dualB(size_t b) const {
	return dual(_input->nR() + b);

}

inline size_t VnsGenerator::violationIfR(size_t r) const {
	size_t result(0);
	for (auto const & decision : *_decisions) {
		result += decision.violation(
				decision.r() == r ?
						!_current._r.contains(decision.r()) :
						_current._r.contains(decision.r()),
				_current._b.contains(decision.b()));
	}
	return result;
}

inline size_t VnsGenerator::violationIfB(size_t b) const {
	size_t result(0);
	for (auto const & decision : *_decisions) {
		result += decision.violation(_current._r.contains(decision.r()),
				(decision.b() == b ?
						!_current._b.contains(decision.b()) :
						_current._b.contains(decision.b())));
	}
	return result;
}
#endif /* GRAPH_HPP */
