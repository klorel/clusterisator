#ifndef VNS_GENERATOR_HPP
#define VNS_GENERATOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "IndexedList.hpp"
#include "VnsGeneratorSolution.hpp"
#include "Decision.hpp"
#include "IOracle.h"

class Node;
class VnsGenerator: public IOracle {
public:
	VnsGenerator(ICliquePartitionProblem const *, DoubleVector const *,
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
	void initialize();
	void backTrack();
	size_t violationIf(size_t id) const;
	size_t swap(size_t id, VnsGeneratorSolution & point);
	void swap(size_t k, Double deltaCost, Double deltaDual);
	void swap(size_t k, bool wasIn, Double deltaCost, Double deltaDual);
	void swap(size_t k);
	bool checkGradient() const;

	bool tryMove(size_t id, Double delta, Double deltaDual);
private:
	IndexedList _allNodes;

	VnsGeneratorSolution _current;
	VnsGeneratorSolution _best;
	DoubleVector _delta;

	DoubleVector _gradient;
	IndexedList _wasSwapped;
};

inline Double VnsGenerator::dual(size_t n) const {
	return (*_dual)[n];
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
