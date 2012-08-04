/*
 * IGraph.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef IGRAPH_HPP_
#define IGRAPH_HPP_

#include <vector>

class IGraph {
public:
	typedef std::pair<size_t, double> Element;
	typedef std::vector<std::pair<size_t, double> > AdjacentList;
	typedef std::vector<AdjacentList*> Container;

	typedef Container::iterator iterator;
	typedef Container::const_iterator const_iterator;
	typedef std::map<size_t, double> TempRow;
	typedef std::vector<TempRow> TempGraph;
public:
	virtual size_t nbNodes() const = 0;
	virtual size_t nbEdges() const = 0;

	virtual double const & degree(size_t const & node) const = 0;
	virtual double const & degree() const = 0;

	virtual AdjacentList const & adjacentList(size_t const & node) const = 0;

	virtual void clear() = 0;

	virtual ~IGraph();
};

inline IGraph::~IGraph() {
}
#endif /* IGRAPH_HPP_ */
