#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "gencol.h"
#include "RectMatrix.hpp"
class BipartiteGraph;
std::ostream & operator<<(std::ostream &out, Graph const&);
class BipartiteGraph {
public:
	RectMatrix _a;
	BipartiteGraph(std::string const &, std::ostream & = std::cout);

	void read(std::string const &, std::ostream & = std::cout);
	virtual ~BipartiteGraph();

	void exportAmpl(std::string const &)const;
};


#endif /* GRAPH_HPP */
