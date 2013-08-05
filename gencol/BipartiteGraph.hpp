#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "gencol.h"
#include "RectMatrix.hpp"

#include "Graph.hpp"

class BipartiteGraph;
std::ostream & operator<<(std::ostream &out, BipartiteGraph const&);
class BipartiteGraph {
public:
	// data R B
	BipartiteGraph();
	BipartiteGraph(std::string const &, std::ostream & = std::cout);

	void read(std::string const &, std::ostream & = std::cout);
	void build();
	virtual ~BipartiteGraph();

	void exportAmpl(std::string const &)const;
	size_t nV()const;
	size_t nR()const;
	size_t nB()const;
	Edges const & edges()const;

	Double w(size_t r, size_t b)const;
	Double kR(size_t )const;
	Double kB(size_t )const;
public:
	Double _m;
	Double _inv_m;

	DoubleVector _kB;
	DoubleVector _kR;
	
	Edges _edges;
	RectMatrix _a;
};


#endif /* GRAPH_HPP */
