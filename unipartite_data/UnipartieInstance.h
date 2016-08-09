/*
 * UnipartieInstance.h
 *
 *  Created on: 6 mars 2014
 *      Author: manuel
 */

#ifndef UNIPARTIEINSTANCE_H_
#define UNIPARTIEINSTANCE_H_

#include "../clustering/IndexedList.h"
#include "../columnsgeneration/ClusteringProblem.h"
#include "common.h"
#include "Edge.h"
#include "gencol.h"
class CliquePartitionProblem;

// (xr)/m - (D/2m)²
class UnipartieInstance: public ClusteringProblem {
public:
	virtual double cst() const;
	virtual int nV() const;
	virtual Edges const & edges() const;

	virtual std::string name(int v) const;
	virtual Double k(int) const;

	virtual void writeSolution(FractionnarySolution const&, double) const;
	virtual Double computeCost(IntSet const &) const;
	virtual Double computeCost(IndexedList const &) const;

	virtual void update(int, IndexedList const &, DoubleVector &) const;
	virtual void gradient(IndexedList const & v, DoubleVector &) const;

	virtual std::vector<Edge> const & costs() const;
	virtual void cpCost(DoubleVector &) const;

	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights & result) const;

public:
	void build();
public:
	Edges & edges();

	Double w(int vi, int vj) const;

	Double m() const;
	Double inv_m() const;
	Double sum_k() const;

public:
	UnipartieInstance();
	UnipartieInstance(Edges const & edges);
	virtual ~UnipartieInstance();

	void getCliquePartitionProblem(CliquePartitionProblem & result) const;
private:
	Double _m;
	Double _inv_m;
	double _cst;
	DoubleVector _k;

	Edges _edges;

//	typedef std::pair<size_t, double> Link;
//	typedef std::vector<Link> Links;
//	typedef std::vector<Links> AllLinks;

	typedef std::map<int, double> Links;
	typedef std::vector<Links> AllLinks;

	AllLinks _allLinks;
	AllLinks _negLinks;
	std::vector<Edge> _costs;

	AdjencyGraph _adjencyGraph;
};

inline double UnipartieInstance::cst() const {
	return _cst;
}
inline std::string UnipartieInstance::name(int v) const {
	return GetStr("YR_", v);
}
inline int UnipartieInstance::nV() const {
	return (int) _k.size();
}

inline Double UnipartieInstance::k(int id) const {
	return _k[id];
}

inline Edges const & UnipartieInstance::edges() const {
	return _edges;
}
inline Edges & UnipartieInstance::edges() {
	return _edges;
}
inline Double UnipartieInstance::w(int vi, int vj) const {
	return 0;
//	return (_a.get(r, b) - _k[r] * _k[nR() + b] / _m) / _m;
}

inline Double UnipartieInstance::m() const {
	return _m;
}
inline Double UnipartieInstance::inv_m() const {
	return _inv_m;
}
inline Double UnipartieInstance::sum_k() const {
	return std::accumulate(_k.begin(), _k.end(), 0.0);
}

inline std::vector<Edge> const & UnipartieInstance::costs() const {
	return _costs;
}

#endif /* UNIPARTIEINSTANCE_H_ */
