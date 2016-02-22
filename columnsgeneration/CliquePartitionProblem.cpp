#include "CliquePartitionProblem.h"

#include "MilpOracle.h"
#include "Node.h"
#include "QpOracle.h"
#include "VnsGenerator.h"

void CliquePartitionProblem::clear() {
	_edges2.clear();
	_costs.clear();
	_allLinks.clear();
	_n = 0;
}
size_t CliquePartitionProblem::nV() const {
	return _n;
}
size_t &CliquePartitionProblem::nV() {
	return _n;
}

Edges & CliquePartitionProblem::getEdges() {
	return _edges2;
}
Edges const & CliquePartitionProblem::getEdges() const {
	return _edges2;
}
AllLinks & CliquePartitionProblem::getAllLinks() {
	return _allLinks;
}
AllLinks const & CliquePartitionProblem::getAllLinks() const {
	return _allLinks;
}
std::map<size_t, double> const & CliquePartitionProblem::allLinks(
		size_t v) const {
	return _allLinks[v];
}
std::vector<Edge> & CliquePartitionProblem::getCosts() {
	return _costs;
}
std::vector<Edge> const & CliquePartitionProblem::getCosts() const {
	return _costs;
}

IOracle * CliquePartitionProblem::newMilpOracle() const {
	return new MilpOracle(this);
}
IOracle * CliquePartitionProblem::newMiqpOracle() const {
	return new QpOracle(this);

}

IOracle * CliquePartitionProblem::newOracle(AvailableOracle oracle,
		DoubleVector const * dual, DecisionList const * decision) const {
	IOracle * result(NULL);
	switch (oracle) {
	case MILP:
		result = new MilpOracle(this);
		break;
	case MIQP:
		result = new QpOracle(this);
		break;
	default:
		break;
	}
	return result;
}
IOracle * CliquePartitionProblem::newVnsOracle(DoubleVector const * dual,
		DecisionList const * decision) const {
	return new VnsGenerator(this);
}

void CliquePartitionProblem::branchingWeights(
		FractionnarySolution const & solution,
		BranchingWeights & weights) const {
// on cherche des arrêtes présentes et semi-présentes dans deux colonnes
	BranchingWeights2 temp;
	std::map<Edge, std::pair<IntSet, IntSet> > toto;
	for (auto const & kvp : solution) {
		//		std::cout << std::setw(6) << kvp.first->id();
		//		std::cout << std::setw(15) << kvp.second;
		//		std::cout << std::endl;
		for (Edge const & e : getCosts()) {
			bool const iFirst(kvp.first->contains(e._i));
			bool const iSecond(kvp.first->contains(e._j));
			if (iFirst && iSecond) {
				toto[e].first.insert(kvp.first->id());
			} else if (iFirst || iSecond) {
				toto[e].second.insert(kvp.first->id());
			}
		}
	}
//	std::cout << "synthese" << std::endl;
	weights.clear();
	for (auto const & t : toto) {
		//		std::cout << std::setw(6) << t.second.first.size();
		//		std::cout << std::setw(6) << t.second.second.size();
		//		std::cout << std::endl;
		if (!t.second.first.empty() && !t.second.second.empty()) {
			//			std::cout << std::setw(6) << t.first._i;
			//			std::cout << std::setw(6) << t.first._j;
			//			std::cout << std::endl;
			weights.insert(
					std::make_pair(
							0.5
									* ((int) ((t.second.first.size()
											+ t.second.second.size()))),
							std::make_pair(t.first._i, t.first._j)));
		}
	}
	if (weights.empty()) {
		std::cout << "Weights.empty(), generating full weights" << std::endl;
		for (auto const & kvp : solution) {
			for (Edge const & e : getCosts()) {
				bool const iR(kvp.first->contains(e._i));
				bool const iB(kvp.first->contains(e._j));
				if (iR && iB) {
					toto[Edge(e._i, e._j, 1)].first.insert(kvp.first->id());
				} else if (iR || iB) {
					toto[Edge(e._i, e._j, 1)].second.insert(kvp.first->id());
				}

			}
		}

		for (auto const & t : toto) {
			//		std::cout << std::setw(6) << t.second.first.size();
			//		std::cout << std::setw(6) << t.second.second.size();
			//		std::cout << std::endl;
			if (!t.second.first.empty() && !t.second.second.empty()) {
				//			std::cout << std::setw(6) << t.first._i;
				//			std::cout << std::setw(6) << t.first._j;
				//			std::cout << std::endl;
				weights.insert(
						std::make_pair(
								0.5
										* ((int) ((t.second.first.size()
												+ t.second.second.size()))),
								std::make_pair(t.first._i, t.first._j)));
			}
		}
		if (weights.empty())
			std::cout << "weights.empty()" << std::endl;
	}
}
#include <cplex.h>

void CliquePartitionProblem::cps(std::string const &fileName) const {
	size_t const n(nV());
	int err;
	CPXENVptr env = CPXopenCPLEX(&err);
	CPXLPptr prob = CPXcreateprob(env, &err, "cps");

	CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
	CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON);

	ColumnBuffer columnBuffer;
	DoubleVector denseCost;
//	cpCost(denseCost);
	IntVector index(n * (n - 1));
	size_t nCols(0);
	for (size_t u(0); u < nV(); ++u) {
		for (size_t v(u + 1); v < nV(); ++v, ++nCols) {
			size_t const id(ijtok(n, u, v));
			columnBuffer.add(denseCost[id], CPX_BINARY, 0, 1,
					GetStr("x_", u, "_", v));
			if (id != nCols) {
				std::cout << "wrong numbering" << std::endl;
				std::exit(-1);
			}
		}
	}

	columnBuffer.add(cst(), CPX_CONTINUOUS, 1, 1, "CST");
	columnBuffer.add(env, prob);
	RowBuffer rowBuffer;
	double const eps(1e-20);
	for (size_t u(0); u < nV(); ++u) {
		for (size_t v(u + 1); v < nV(); ++v) {
			size_t const uv(ijtok(n, u, v));
			for (size_t w(v + 1); w < nV(); ++w) {
				size_t const uw(ijtok(n, u, w));
				size_t const vw(ijtok(n, v, w));
				if (denseCost[uv] > -eps || denseCost[vw] > -eps) {
					rowBuffer.add(1, 'L', GetStr("T_", u, "_", v, "_", w));
					rowBuffer.add(uv, 1);
					rowBuffer.add(vw, 1);
					rowBuffer.add(uw, -1);
				}
				if (denseCost[vw] > -eps || denseCost[uw] > -eps) {
					rowBuffer.add(1, 'L', GetStr("T_", v, "_", w, "_", u));
					rowBuffer.add(vw, 1);
					rowBuffer.add(uw, 1);
					rowBuffer.add(uv, -1);
				}
				if (denseCost[uw] > -eps || denseCost[uv] > -eps) {
					rowBuffer.add(1, 'L', GetStr("T_", w, "_", u, "_", v));
					rowBuffer.add(uw, 1);
					rowBuffer.add(uv, 1);
					rowBuffer.add(vw, -1);
				}
			}
		}
	}

	rowBuffer.add(env, prob);
	CPXchgobjsen(env, prob, -1);

	CPXwriteprob(env, prob, (fileName + ".lp").c_str(), "LP");

	CPXsetintparam(env, CPX_PARAM_SOLUTIONTARGET,
	CPX_SOLUTIONTARGET_OPTIMALGLOBAL);
	CPXsetintparam(env, CPX_PARAM_CUTPASS, -1);

	CPXmipopt(env, prob);

	double objval;
	CPXgetobjval(env, prob, &objval);
	std::cout << "optimal solution value  : " << std::setprecision(20) << objval
			<< std::endl;

	CPXcloseCPLEX(&env);
	CPXfreeprob(env, &prob);

}
Double CliquePartitionProblem::computeCost(IndexedList const &v) const {
	Double result(0);
	size_t const n(nV());
	for (auto i : v) {
		for (auto j : v) {
			if (i < j) {
				size_t const ij(ijtok(n, i, j));
				result += _costs[ij]._v;
			}
		}
	}
	return result;
}

Double CliquePartitionProblem::computeCost(std::set<size_t> const & v) const {
	Double result(0);
	size_t const n(nV());
	for (auto i : v) {
		for (auto j : v) {
			if (i < j) {
				size_t const ij(ijtok(n, i, j));
				result += _costs[ij]._v;
			}
		}
	}
	return result;

}

void CliquePartitionProblem::update(size_t id, bool wasIn,
		DoubleVector & gradient) const {
	for (auto const & link : _allLinks[id]) {
		if (wasIn)
			gradient[link.first] -= link.second;
		else
			gradient[link.first] += link.second;
	}
}

void CliquePartitionProblem::gradient(IndexedList const & v,
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

void CliquePartitionProblem::writeSolution(
		FractionnarySolution const& bestSolution, double lb) const {
	std::ofstream file(
			GetStr("optimal/", problemName(), "_", lb, ".txt").c_str());
	for (auto const & c : bestSolution) {
		for (auto const & edge : getCosts()) {
			size_t const r(edge._i);
			size_t const b(edge._j);
			if (c.first->contains(r) && c.first->contains(b)) {
				file << std::setw(6) << 1 + r;
				file << std::setw(6) << 1 + b;
				file << std::endl;
			}

		}
//		for (size_t r(0); r < nR(); ++r) {
//			for (size_t b(0); b < nB(); ++b) {
//				if (c.first->contains(r) && c.first->contains(nR() + b)) {
//					file << std::setw(6) << 1 + r;
//					file << std::setw(6) << 1 + b + nR();
//					file << std::endl;
//				}
//
//			}
//		}
	}
	file.close();
}
