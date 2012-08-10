#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "common.h"
#include "ILinks.hpp"

class Graph: public ILinks {
public:
	Graph(size_t = 0);
	Graph(std::string const &, std::ostream * = 0);

	virtual ~Graph();

	void allocate(size_t);

	void buildDegrees();

	void read(std::string const &, std::ostream * = 0);

	double const & squareDegree() const;

	size_t & nbLinks(void);
	size_t nbObs() const;
	size_t nbLinks(void) const;

	void clear();

	double const & weight(size_t) const;

	LinksIterator links(size_t) const;
	double const & link(size_t i, size_t j) const;
private:
	bool _isEdgeWeighted;

	size_t _nbEdge;

	IntVector _begin;
	IntVector _size;
	Links _edges;
	DoubleVector _nodeWeights;
};

#endif /* GRAPH_HPP */
