#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "common.h"
#include "RectMatrix.hpp"
#include "Edge.h"

class BipartiteGraph;
std::ostream & operator<<(std::ostream &out, BipartiteGraph const&);
class BipartiteGraph {
public:
	// data R B
	BipartiteGraph();
	BipartiteGraph(Edges const & edges);
	void build();
	virtual ~BipartiteGraph();

	size_t nV() const;
	size_t nR() const;
	size_t nB() const;

	Edges & edges();
	Edges const & edges() const;
	RectMatrix const & a() const;
	Double w(size_t r, size_t b) const;
	Double a(size_t r, size_t b) const;
	Double kR(size_t) const;
	Double kB(size_t) const;

	Double m() const;
	Double inv_m() const;
	Double sum_kR() const;
	Double sum_kB() const;

	std::map<size_t, double> const & allLinks(size_t v) const;
	std::map<size_t, double> const & posLinks(size_t v) const;
	std::map<size_t, double> const & negLinks(size_t v) const;

	virtual std::string name() const;

	Double computeCost(std::set<size_t> const &) const;
public:
	Double _m;
	Double _inv_m;
	DoubleVector _kR;
	DoubleVector _kB;

	Edges _edges;
	RectMatrix _a;
	std::vector<std::map<size_t, double> > _allLinks;
	std::vector<std::map<size_t, double> > _posLinks;
	std::vector<std::map<size_t, double> > _negLinks;
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
inline RectMatrix const & BipartiteGraph::a() const {
	return _a;
}

inline Edges const & BipartiteGraph::edges() const {
	return _edges;
}
inline Edges & BipartiteGraph::edges() {
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
