#include "BipartiteGraph.h"

#include "../columnsgeneration/MilpOracle.h"
#include "../columnsgeneration/QpOracle.h"
#include "../columnsgeneration/CliquePartitionProblem.h"
#include "BinaryDecompositionOracle.h"

BipartiteGraph::BipartiteGraph() :
		_m(0), _inv_m(0), _edges(), _a() {

}

BipartiteGraph::BipartiteGraph(Edges const & edges) :
		_m(0), _inv_m(0), _edges(edges), _a() {
	build();
}

BipartiteGraph::~BipartiteGraph() {
}
void BipartiteGraph::build() {
	int R(0);
	int B(0);
//	std::cout << _edges << std::endl;
	for (auto const & e : _edges) {
		R = std::max(R, e._i);
		B = std::max(B, e._j);
	}
	_a.allocate(R + 1, B + 1);
	_k.assign(R + 1 + B + 1, 0);
	for (auto const & e : _edges) {
		_a.set(e._i, e._j, e._v);
		_k[e._i] += e._v;
		_k[nR() + e._j] += e._v;
		_m += e._v;
	}
	_inv_m = 1.0 / _m;
	_allLinks.assign(nV(), std::map<int, double>());
	_costs.reserve(nR() * nB());

	for (int r(0); r < nR(); ++r) {
		for (int b(0); b < nB(); ++b) {
			double const value(w(r, b));
			if (value != 0) {
				_costs.push_back(Edge(r, nR() + b, value));
				_allLinks[r][nR() + b] = value;
				_allLinks[nR() + b][r] = value;
			}
		}
	}
//	std::exit(0);
//	MY_PRINT(_m);
//	MY_PRINT(_inv_m);
//	MY_PRINT(R);
//	MY_PRINT(B);
//	MY_PRINT(nV());
//	MY_PRINT(nR());
//	MY_PRINT(nB());
}
void BipartiteGraph::cpCost(DoubleVector &result) const {
	int const n(nV());
	result.assign(n * (n - 1) / 2, 0);
	for (int r(0); r < nR(); ++r) {
		for (int b(0); b < nB(); ++b) {
			double const value(w(r, b));
			if (value != 0) {
				int const ij(ijtok(n, r, nR() + b));
				result[ij] = value;
			}
		}
	}
}
//void BipartiteGraph::read(std::string const & fileName, std::ostream & stream) {
//	std::ifstream file(fileName.c_str());
//	//_a << file;
//	std::string line;
//	std::getline(file, line);
//	size_t r;
//	size_t b;
//	{
//		std::stringstream buffer(line);
//		buffer >> r;
//		buffer >> b;
//	}
//	_a.allocate(r, b);
//	while (std::getline(file, line)) {
//		std::stringstream buffer(line);
//		buffer >> r;
//		buffer >> b;
//		_a.get(r - 1, b - 1) = 1;
//	}
//	file.close();
//	build();
//}

Double BipartiteGraph::computeCost(IndexedList const &v) const {
	Double result(0);
	for (auto const & r : v) {
		for (auto const & b : v) {
			if (r < b && r < nR() && b >= nR())
				result += w(r, b - nR());
		}
	}
	return result;
}

Double BipartiteGraph::computeCost(IntSet const & v) const {
	Double result(0);
	for (auto const & r : v) {
		if (r < nR()) {
			for (auto const & b : v) {
				if (b >= nR()) {
					result += w(r, b - nR());
				}
			}
		}
	}
	return result;

}

void BipartiteGraph::writeSolution(FractionnarySolution const& bestSolution,
		double lb) const {
	std::ofstream file(
			GetStr("optimal/", problemName(), "_", lb, ".txt").c_str());
	for (auto const & c : bestSolution) {
		for (auto const & edge : costs()) {
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

void BipartiteGraph::getCliquePartitionProblem(
		CliquePartitionProblem & result) const {
	result.clear();
	result.nV() = nV();
	result.getEdges() = _edges;
	result.getCosts() = _costs;
	result.getAllLinks() = _allLinks;
}

void BipartiteGraph::branchingWeights(FractionnarySolution const & solution,
		BranchingWeights & weights) const {
// on cherche des arrêtes présentes et semi-présentes dans deux colonnes
	BranchingWeights2 temp;
	std::map<Edge, std::pair<IntSet, IntSet> > toto;
	for (auto const & kvp : solution) {
		//		std::cout << std::setw(6) << kvp.first->id();
		//		std::cout << std::setw(15) << kvp.second;
		//		std::cout << std::endl;
		for (Edge const & e : _costs) {
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
			for (Edge const & e : _costs) {
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
