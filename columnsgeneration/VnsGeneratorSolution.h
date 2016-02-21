#ifndef VNS_GENERATOR_SOLUTION_HPP
#define VNS_GENERATOR_SOLUTION_HPP

#include "../clustering/IndexedList.h"
#include "Column.h"
#include "Decision.h"
#include "gencol.h"

class Node;
class VnsGeneratorSolution {
public:
	VnsGeneratorSolution(ICliquePartitionProblem const *,
			DoubleVector const * dual);
	virtual ~VnsGeneratorSolution();
	VnsGeneratorSolution & operator=(VnsGeneratorSolution const &);
public:
	void swap(size_t id);
	void swap(size_t id, Double, Double);

	Double computeCost() const;
	Double computeReducedCost() const;
	void build(Column &);
	size_t violation(DecisionList const &) const;
	void clear();
	bool check() const;
public:
	static bool IsBetter(VnsGeneratorSolution const & p,
			VnsGeneratorSolution const & q, DecisionList const & decisions);
	Double dual(size_t) const;
private:
	ICliquePartitionProblem const * _input;
	DoubleVector const * _dual;
public:
	IndexedList _v;

	Double _cost;
	Double _reducedCost;
};

inline Double VnsGeneratorSolution::dual(size_t n) const {
	return (*_dual)[n];
}
#endif /* GRAPH_HPP */
