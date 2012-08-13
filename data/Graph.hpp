#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../utils/common.h"
#include "../interfaces/ILinks.hpp"

class Graph: public ILinks {
public:
	Graph(size_t = 0);
	Graph(std::string const &);

	virtual ~Graph();

	void allocate(size_t);

	void buildSumOfLinks();

	void read(std::string const &);

	Double squareDegree() const;

	size_t & nbLinks(void);
	size_t nbObs() const;
	size_t nbLinks(void) const;

	void clear();

	Double weight(size_t) const;

	LinksIterator links(size_t) const;
	Double link(size_t i, size_t j) const;

	Double sumOfNodeLinks(size_t i) const;
private:
	bool _isEdgeWeighted;

	size_t _nbEdge;

	IntVector _begin;
	IntVector _size;
	Links _edges;
	DoubleVector _nodeWeights;
	DoubleVector _sumOfLinks;
};

#endif /* GRAPH_HPP */
