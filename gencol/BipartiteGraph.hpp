#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "gencol.h"
#include "RectMatrix.hpp"

#include "Graph.hpp"

class BipartiteGraph;
std::ostream & operator<<(std::ostream &out, BipartiteGraph const&);
class BipartiteGraph : public Graph {
public:
	// data R B
	RectMatrix _a;
	BipartiteGraph();
	BipartiteGraph(std::string const &, std::ostream & = std::cout);

	void read(std::string const &, std::ostream & = std::cout);
	void build();
	virtual ~BipartiteGraph();

	void exportAmpl(std::string const &)const;

	size_t nR()const;
	size_t nB()const;

	DoubleVector _kB;
	DoubleVector _kR;
};


#endif /* GRAPH_HPP */
