#ifndef VNS_GENERATOR_HPP
#define VNS_GENERATOR_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "Column.hpp"
#include "IndexedList.hpp"
#include "VnsGeneratorSolution.hpp"
#include "Decision.hpp"
#include "IOracle.h"

class Node;
class VnsGenerator: public IOracle {
public:
	VnsGenerator(BipartiteGraph const *, DoubleVector const *,
			DecisionList const *);
	virtual ~VnsGenerator();
public:
	virtual bool run(size_t iteMax, bool stopAtFirst);
public:
	bool check(bool alsoDecision = false) const;
	void shake(size_t k);
	void compute();
	bool localSearch();

	Double dual(size_t) const;
	Double dualR(size_t) const;
	Double dualB(size_t) const;
	Double deltaR(size_t) const;
	Double deltaB(size_t) const;

	void initialize();

	size_t violationIfR(size_t r) const;
	size_t violationIfB(size_t b) const;
	size_t violationIf(size_t id) const;
private:
	IndexedList _allNodes;

	VnsGeneratorSolution _current;
	VnsGeneratorSolution _best;
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

inline size_t VnsGenerator::violationIf(size_t id) const {
	size_t result(0);
	for (auto const & decision : *_decisions) {
		result += decision.violation(
				decision.noeud1() == id ?
						!_current._v.contains(decision.noeud1()) :
						_current._v.contains(decision.noeud1()),
				(decision.noeud2() == id ?
						!_current._v.contains(decision.noeud2()) :
						_current._v.contains(decision.noeud2())));
	}
	return result;
}
#endif /* GRAPH_HPP */
