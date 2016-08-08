#ifndef VNS_GENERATOR_HPP
#define VNS_GENERATOR_HPP

#include "../clustering/IndexedList.h"
#include "Column.h"
#include "Decision.h"
#include "gencol.h"
#include "IOracle.h"
#include "VnsGeneratorSolution.h"

class Node;
class VnsGenerator: public IOracle {
public:
	VnsGenerator(ClusteringProblem const *);
	virtual ~VnsGenerator();
public:
	virtual bool run(int iteMax, bool stopAtFirst);
	virtual void setData(DoubleVector const &, DecisionList const &);
public:
	bool check(bool alsoDecision = false) const;
	void shake(int k);
	void compute();
	bool localSearch();

	Double dual(int) const;
	void initialize();
	void backTrack();
	int violationIf(int id) const;
	int swap(int id, VnsGeneratorSolution & point);
	void swap(int k, Double deltaCost, Double deltaDual);
	void swap(int k, bool wasIn, Double deltaCost, Double deltaDual);
	void swap(int k);
	bool checkGradient() const;

	bool tryMove(int id, Double delta, Double deltaDual);
private:
	IndexedList _allNodes;

	VnsGeneratorSolution _current;
	VnsGeneratorSolution _best;
	DoubleVector _delta;

	DoubleVector _gradient;
	IndexedList _wasSwapped;

};

inline Double VnsGenerator::dual(int n) const {
	return (*_dual)[n];
}

inline int VnsGenerator::violationIf(int id) const {
	int result(0);
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
