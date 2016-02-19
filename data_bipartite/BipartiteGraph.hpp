#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "common.h"
#include "RectMatrix.hpp"
#include "Edge.h"
#include "IndexedList.hpp"
#include "gencol.h"
#include "ICliquePartitionProblem.h"

class IOracle;
class Node;
//std::ostream & operator<<(std::ostream &out, bipartite::BipartiteGraph const&);

class BipartiteGraph: public ICliquePartitionProblem {
public:
	virtual size_t nV() const;
	virtual Edges const & edges() const;

	virtual std::string name(size_t v) const;
	virtual Double k(size_t) const;

	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const;

	virtual void writeSolution(FractionnarySolution const&, double) const;
	virtual Double computeCost(std::set<size_t> const &) const;
	virtual Double computeCost(IndexedList const &) const;
	virtual Double gradient(size_t id, IndexedList const & v) const;
	virtual void update(size_t id, bool wasIn, DoubleVector &gradient) const;
	virtual void gradient(IndexedList const & v, DoubleVector &) const;
	virtual std::vector<Edge> const & costs() const;
public:
	BipartiteGraph();
	BipartiteGraph(Edges const & edges);
	void build();
	virtual ~BipartiteGraph();

public:
	size_t nR() const;
	size_t nB() const;

	Edges & edges();
	RectMatrix const & a() const;
	Double w(size_t r, size_t b) const;
	Double a(size_t r, size_t b) const;

	Double m() const;
	Double inv_m() const;
	Double sum_kR() const;
	Double sum_kB() const;

	std::map<size_t, double> const & allLinks(size_t v) const;

public:
	Double _m;
	Double _inv_m;
	DoubleVector _k;

	Edges _edges;
	RectMatrix _a;
	std::vector<std::map<size_t, double> > _allLinks;
	std::vector<Edge> _costs;
};
inline std::string BipartiteGraph::name(size_t v) const {
	return v < nR() ? GetStr("YR_", v) : GetStr("YB_", v - nR());
}
inline size_t BipartiteGraph::nR() const {
	return _a.getN();
}
inline size_t BipartiteGraph::nB() const {
	return _a.getM();
}
inline size_t BipartiteGraph::nV() const {
	return _k.size();
}

inline Double BipartiteGraph::k(size_t id) const {
	return _k[id];
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
	return (_a.get(r, b) - _k[r] * _k[nR() + b] / _m) / _m;
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
	return std::accumulate(_k.begin(), _k.begin() + nR(), 0);
}
inline Double BipartiteGraph::sum_kB() const {
	return std::accumulate(_k.begin() + nR(), _k.end(), 0);
}
inline std::map<size_t, double> const & BipartiteGraph::allLinks(
		size_t v) const {
//	MY_PRINT(v);
//	MY_PRINT(nV());
	return _allLinks[v];
}

inline Double BipartiteGraph::gradient(size_t id, IndexedList const & v) const {
	Double result(0);
	if (id < nR()) {
		for (size_t b(0); b < nB(); ++b) {
			if (v.contains(nR() + b))
				result += w(id, b);
		}
	} else {
		for (size_t r(0); r < nR(); ++r) {
			if (v.contains(r))
				result += w(r, id - nR());
		}
	}
	return result;
}
inline void BipartiteGraph::update(size_t id, bool wasIn,
		DoubleVector & gradient) const {
	for (auto const & link : _allLinks[id]) {
		if (wasIn)
			gradient[link.first] -= link.second;
		else
			gradient[link.first] += link.second;
	}
}

inline std::vector<Edge> const & BipartiteGraph::costs() const {
	return _costs;
}
inline void BipartiteGraph::gradient(IndexedList const & v,
		DoubleVector & result) const {
	result.assign(v.max_size(), 0);
	for (auto const & e : _costs) {
		if (v.contains(e._j)) {
			result[e._i] += e._v;
		}
		if (v.contains(e._i)) {
			result[e._j] += e._v;
		}
	}
}

#endif /* GRAPH_HPP */
