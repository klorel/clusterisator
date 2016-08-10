#include "CliquePartitionProblem.h"

#include "MilpOracle.h"
#include "Node.h"
#include "QpOracle.h"
#include "VnsGenerator.h"
#include "ILpSolver.h"


void CliquePartitionProblem::clear() {
	_edges2.clear();
	_costs.clear();
	_allLinks.clear();
	_n = 0;
}
int CliquePartitionProblem::nV() const {
	return _n;
}
int &CliquePartitionProblem::nV() {
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
std::map<int, double> const & CliquePartitionProblem::allLinks(int v) const {
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

//#include <cplex.h>
//
void CliquePartitionProblem::cps(std::string const &fileName, ILpSolver & solver) const {
	int const n(nV());
	solver.add(std::cout);
	solver.initLp(fileName);

//
	char const binary(solver.binary());
	char const continuous(solver.continuous());
	char const leq(solver.leq());
	char const eq(solver.eq());
	char const geq(solver.geq());
	//
	ColumnBuffer columnBuffer(continuous);
	DoubleVector denseCost(n * (n - 1), 0);
	for (auto const & e : _costs) {
		int const u(e._i);
		int const v(e._j);
		int const id(ijtok(n, u, v));
		denseCost[id] = e._v;
	}
	//cpCost(denseCost);
	//IntVector index(n * (n - 1));
	int nCols(0);
	for (int u(0); u < nV(); ++u) {
		for (int v(u + 1); v < nV(); ++v, ++nCols) {
			int const id(ijtok(n, u, v));
			
			columnBuffer.add(denseCost[id], binary, 0, 1,
					GetStr("x_", u, "_", v));
			if (id != nCols) {
				std::cout << "wrong numbering" << std::endl;
				std::exit(-1);
			}
		}
	}
	columnBuffer.add(cst(), continuous, 1, 1, "CST");
	solver.add(columnBuffer);

	RowBuffer rowBuffer;
	double const eps(1e-20);
	for (int u(0); u < nV(); ++u) {
		for (int v(u + 1); v < nV(); ++v) {
			int const uv(ijtok(n, u, v));
			for (int w(v + 1); w < nV(); ++w) {
				int const uw(ijtok(n, u, w));
				int const vw(ijtok(n, v, w));
				if (denseCost[uv] > -eps || denseCost[vw] > -eps) {
					rowBuffer.add(1, leq, GetStr("T_", u, "_", v, "_", w));
					rowBuffer.add(uv, 1);
					rowBuffer.add(vw, 1);
					rowBuffer.add(uw, -1);
				}
				if (denseCost[vw] > -eps || denseCost[uw] > -eps) {
					rowBuffer.add(1, leq, GetStr("T_", v, "_", w, "_", u));
					rowBuffer.add(vw, 1);
					rowBuffer.add(uw, 1);
					rowBuffer.add(uv, -1);
				}
				if (denseCost[uw] > -eps || denseCost[uv] > -eps) {
					rowBuffer.add(1, leq, GetStr("T_", w, "_", u, "_", v));
					rowBuffer.add(uw, 1);
					rowBuffer.add(uv, 1);
					rowBuffer.add(vw, -1);
				}
			}
		}
	}
	solver.add(rowBuffer);
	solver.maximize();
	//solver.write(fileName + ".lp");

	solver.run();

	double objval = solver.objValue();
	std::cout << "optimal solution value  : " << std::setprecision(20) << objval << std::endl;

}

Double CliquePartitionProblem::computeCost(IndexedList const &v) const {
	Double result(0);
	int const n(nV());
	for (auto i : v) {
		for (auto j : v) {
			if (i < j) {
				int const ij(ijtok(n, i, j));
				result += _costs[ij]._v;
			}
		}
	}
	return result;
}

Double CliquePartitionProblem::computeCost(IntSet const & v) const {
	Double result(0);
	int const n(nV());
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

void CliquePartitionProblem::update(int id, IndexedList const & v,
		DoubleVector & gradient) const {
	bool const wasIn(v.contains(id));
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
			int const r(edge._i);
			int const b(edge._j);
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
