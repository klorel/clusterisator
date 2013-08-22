#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "gencol.h"
#include "RectMatrix.hpp"
#include "Edge.hpp"

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

	void exportAmpl(std::string const &) const;
	size_t nV() const;
	size_t nR() const;
	size_t nB() const;
	Edges const & edges() const;

	Double w(size_t r, size_t b) const;
	Double a(size_t r, size_t b) const;
	Double kR(size_t) const;
	Double kB(size_t) const;

	Double m() const;
	Double inv_m() const;
	Double sum_kR() const;
	Double sum_kB() const;

	virtual std::string name() const {
		return "";
	}
public:
	Double _m;
	Double _inv_m;
	DoubleVector _kR;
	DoubleVector _kB;

	Edges _edges;
	RectMatrix _a;
};

inline size_t BipartiteGraph::nR() const {
	return _a.getN();
}
inline size_t BipartiteGraph::nB() const {
	return _a.getM();
}
inline size_t BipartiteGraph::nV() const {
	return nR() + nB();
}

inline Double BipartiteGraph::kR(size_t r) const {
	return _kR[r];
}
inline Double BipartiteGraph::kB(size_t b) const {
	return _kB[b];
}

inline Edges const & BipartiteGraph::edges() const {
	return _edges;
}
inline Double BipartiteGraph::w(size_t r, size_t b) const {
	return (_a.get(r, b) - _kR[r] * _kB[b] / _m) / _m;
}
inline Double BipartiteGraph::a(size_t r, size_t b) const {
	return _a.get(r, b);
}

inline Double BipartiteGraph::m() const {
	return _m;
}
inline Double BipartiteGraph::inv_m() const {
	return _inv_m;
}
inline Double BipartiteGraph::sum_kR() const {
	return std::accumulate(_kR.begin(), _kR.end(), 0);
}
inline Double BipartiteGraph::sum_kB() const {
	return std::accumulate(_kB.begin(), _kB.end(), 0);
}
#endif /* GRAPH_HPP */
