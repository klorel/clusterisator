/*
 * Edge.hpp
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef EDGE_HPP_
#define EDGE_HPP_

#include "common.h"

class Edge;
typedef std::list<Edge> Edges;

class Edge {
public:
	size_t _i;
	size_t _j;
	Double _v;
	Edge(size_t i, size_t j, Double v) :
			_i(i), _j(j), _v(v) {
	}
	bool operator<(Edge const & rhs) const {

		return std::less<std::pair<size_t, size_t>>()(std::make_pair(_i, _j),
				std::make_pair(rhs._i, rhs._j));
	}
};

#endif /* EDGE_HPP_ */
