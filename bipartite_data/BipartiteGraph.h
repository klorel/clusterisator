#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "../clustering/IndexedList.h"
#include "../clustering/RectMatrix.h"
#include "../columnsgeneration/ClusteringProblem.h"
#include "common.h"
#include "Edge.h"
#include "gencol.h"

class IOracle;
class Node;
//std::ostream & operator<<(std::ostream &out, bipartite::BipartiteGraph const&);
class CliquePartitionProblem;
class BipartiteGraph: public ClusteringProblem {
public:
	virtual int nV() const;
	virtual Edges const & edges() const;

	virtual std::string name(int v) const;
	virtual Double k(int) const;

	virtual void writeSolution(FractionnarySolution const&, double) const;
	virtual Double computeCost(IntSet const &) const;
	virtual Double computeCost(IndexedList const &) const;

	virtual void update(int id, IndexedList const & v, DoubleVector &) const;
	virtual void gradient(IndexedList const & v, DoubleVector &) const;
	virtual std::vector<Edge> const & costs() const;
	virtual void cpCost(DoubleVector &) const;

	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights & result) const;

public:
	BipartiteGraph();
	BipartiteGraph(Edges const & edges);
	void build();
	virtual ~BipartiteGraph();

public:
	int nR() const;
	int nB() const;

	Edges & edges();
	RectMatrix const & a() const;
	Double w(int r, int b) const;
	Double a(int r, int b) const;

	Double m() const;
	Double inv_m() const;
	Double sum_kR() const;
	Double sum_kB() const;

	std::map<int, double> const & allLinks(int v) const;

	void getCliquePartitionProblem(CliquePartitionProblem & result) const;
public:
	Double _m;
	Double _inv_m;
	DoubleVector _k;

	Edges _edges;
	RectMatrix _a;
	std::vector<std::map<int, double> > _allLinks;
	std::vector<Edge> _costs;

};
inline std::string BipartiteGraph::name(int v) const {
	return v < nR() ? GetStr("YR_", v) : GetStr("YB_", v - nR());
}
inline int BipartiteGraph::nR() const {
	return _a.getN();
}
inline int BipartiteGraph::nB() const {
	return _a.getM();
}
inline int BipartiteGraph::nV() const {
	return (int)_k.size();
}

inline Double BipartiteGraph::k(int id) const {
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
inline Double BipartiteGraph::w(int r, int b) const {
	return (_a.get(r, b) - _k[r] * _k[nR() + b] / _m) / _m;
}
inline Double BipartiteGraph::a(int r, int b) const {
	return _a.get(r, b);
}

inline Double BipartiteGraph::m() const {
	return _m;
}
inline Double BipartiteGraph::inv_m() const {
	return _inv_m;
}
inline Double BipartiteGraph::sum_kR() const {
	return std::accumulate(_k.begin(), _k.begin() + nR(), 0.0);
}
inline Double BipartiteGraph::sum_kB() const {
	return std::accumulate(_k.begin() + nR(), _k.end(), 0.0);
}
inline std::map<int, double> const & BipartiteGraph::allLinks(
	int v) const {
//	MY_PRINT(v);
//	MY_PRINT(nV());
	return _allLinks[v];
}

inline std::vector<Edge> const & BipartiteGraph::costs() const {
	return _costs;
}
inline void BipartiteGraph::update(int id, IndexedList const & v,
		DoubleVector & gradient) const {
	bool const wasIn(v.contains(id));
	for (auto const & link : _allLinks[id]) {
		if (wasIn)
			gradient[link.first] -= link.second;
		else
			gradient[link.first] += link.second;
	}
}

inline void BipartiteGraph::gradient(IndexedList const & v,
		DoubleVector & result) const {
	ASSERT_CHECK(v.max_size()==nV());
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
