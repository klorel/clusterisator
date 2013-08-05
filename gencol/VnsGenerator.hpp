#ifndef VNS_GENERATOR_HPP
#define VNS_GENERATOR_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"
#include "Column.hpp"
#include "IndexedList.hpp"

class VnsGeneratorSolution{
public:
	VnsGeneratorSolution(BipartiteGraph const &, DoubleVector const & dual);
	~VnsGeneratorSolution();
	VnsGeneratorSolution & operator=(VnsGeneratorSolution const &);
public:
	void swap(size_t id);
	Double delta(size_t id);
	void build(Column &);
private:
	BipartiteGraph const & _input;
	DoubleVector const & _dual;
public:
	IndexedList _v;
	IndexedList _r;
	IndexedList _b;

	Double _cost;
	Double _reducedCost;
};

class VnsGenerator{
public:
	class Solution;
public:
	VnsGenerator(BipartiteGraph const & input, DoubleVector const & dual);
	~VnsGenerator();
	
	void shake(VnsGeneratorSolution & solution, size_t k);
	void compute(VnsGeneratorSolution &);
	bool localSearch(VnsGeneratorSolution &);
	bool run();
	std::set<Column > const & columns()const;
private:

	BipartiteGraph const & _input;
	DoubleVector const & _dual;
	
	IndexedList _allNodes;

	std::set<Column > _columns;
	// pour chaque noeud la liste des noeuds connectées
	std::vector< IntSet > _graph;
};

#endif /* GRAPH_HPP */
