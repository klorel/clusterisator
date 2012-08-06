#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "common.h"
#include "IGraph.hpp"

class Graph: public IGraph {
public:

	Graph(size_t const & = 0);
	Graph(std::string const &, std::ostream * = 0);
	Graph(Graph const &);

	virtual ~Graph();

	void fill(TempGraph const &);

	void allocate(size_t const &);
	void desallocate();
	void readRotta(std::string const &);
	void buildDegrees();

	void read(std::string const &, std::ostream * = 0);

	double const & degree(size_t const &) const;
	double const & degree() const;
	double const & squareDegree() const;

	size_t & nbEdges(void);
	size_t nbNodes() const;
	size_t nbEdges(void) const;

	void clear();
	void writeCplusplus(std::string const &) const;

	AdjacentList & adjacentList(size_t const &);
	AdjacentList const & adjacentList(size_t const &) const;

private:
	Container _rows;
	DoubleVector _degrees;
	double _degree;
	double _squareDegree;
	size_t _nbEdge;

};

#endif /* GRAPH_HPP */
