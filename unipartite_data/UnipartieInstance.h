/*
 * UnipartieInstance.h
 *
 *  Created on: 6 mars 2014
 *      Author: manuel
 */

#ifndef UNIPARTIEINSTANCE_H_
#define UNIPARTIEINSTANCE_H_

#include "../clustering/IndexedList.h"
#include "../columnsgeneration/CliquePartitionProblem.h"
#include "common.h"
#include "Edge.h"
#include "gencol.h"

class UnipartieInstance: public CliquePartitionProblem {
public:
	virtual IOracle * newOracle(AvailableOracle oracle,
			DoubleVector const * dual, DecisionList const * decision) const;
public:
	virtual double cst() const;
	virtual size_t nV() const;
	virtual Edges const & edges() const;

	virtual std::string name(size_t v) const;
	virtual Double k(size_t) const;

	virtual void writeSolution(FractionnarySolution const&, double) const;
	virtual Double computeCost(std::set<size_t> const &) const;
	virtual Double computeCost(IndexedList const &) const;

	virtual void update(size_t id, bool wasIn, DoubleVector &gradient) const;
	virtual void gradient(IndexedList const & v, DoubleVector &) const;

	virtual std::vector<Edge> const & costs() const;
	virtual void cpCost(DoubleVector &) const;
public:
	void build();
public:
	Edges & edges();

	Double w(size_t vi, size_t vj) const;

	Double m() const;
	Double inv_m() const;
	Double sum_k() const;

public:
	UnipartieInstance();
	UnipartieInstance(Edges const & edges);
	virtual ~UnipartieInstance();
private:
	Double _m;
	Double _inv_m;
	double _cst;
	DoubleVector _k;

	Edges _edges;

//	typedef std::pair<size_t, double> Link;
//	typedef std::vector<Link> Links;
//	typedef std::vector<Links> AllLinks;

	typedef std::map<size_t, double> Links;
	typedef std::vector<Links> AllLinks;

	AllLinks _allLinks;
	std::vector<Edge> _costs;
};

inline double UnipartieInstance::cst() const {
	return _cst;
}
inline std::string UnipartieInstance::name(size_t v) const {
	return GetStr("YR_", v);
}
inline size_t UnipartieInstance::nV() const {
	return _k.size();
}

inline Double UnipartieInstance::k(size_t id) const {
	return _k[id];
}

inline Edges const & UnipartieInstance::edges() const {
	return _edges;
}
inline Edges & UnipartieInstance::edges() {
	return _edges;
}
inline Double UnipartieInstance::w(size_t vi, size_t vj) const {
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
	return std::accumulate(_k.begin(), _k.end(), 0);
}

inline void UnipartieInstance::update(size_t id, bool wasIn,
		DoubleVector & gradient) const {
	for (auto const & link : _allLinks[id]) {
		if (wasIn)
			gradient[link.first] -= link.second;
		else
			gradient[link.first] += link.second;
	}
}

inline std::vector<Edge> const & UnipartieInstance::costs() const {
	return _costs;
}
inline void UnipartieInstance::gradient(IndexedList const & v,
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

#endif /* UNIPARTIEINSTANCE_H_ */
