#include "UnipartieInstance.h"

#include "../columnsgeneration/Column.h"
#include "../columnsgeneration/MilpOracle.h"
#include "../columnsgeneration/QpOracle.h"
#include "UnipartiteBinaryDecompositionOracle.h"

UnipartieInstance::UnipartieInstance() :
		_m(0), _inv_m(0), _cst(0), _edges() {

}

UnipartieInstance::UnipartieInstance(Edges const & edges) :
		_m(0), _inv_m(0), _cst(0), _edges(edges) {
	build();
}

UnipartieInstance::~UnipartieInstance() {
}
void UnipartieInstance::build() {
	size_t n(0);
	_m = 0;
	for (auto const & edge : _edges) {
		n = std::max(n, edge._i);
		n = std::max(n, edge._j);
		_m += edge._v;
	}
	n += 1;
	_k.assign(n, 0);
	for (auto const & edge : _edges) {
		_k[edge._i] += edge._v;
		_k[edge._j] += edge._v;
	}
	_inv_m = 1.0 / _m;
	_costs.resize(n * (n - 1) / 2);
	_allLinks.assign(nV(), Links());

	for (size_t i(0); i < n; ++i) {
		for (size_t j(i + 1); j < n; ++j) {
			size_t const ij(ijtok(n, i, j));
			_costs[ij] = Edge(i, j, -_k[i] * _k[j] * _inv_m / 2.0);
		}
	}
	for (auto const & edge : _edges) {
		size_t const ij(ijtok(n, edge._i, edge._j));
		_costs[ij]._v += 1;
	}
	for (size_t i(0); i < n; ++i) {
		for (size_t j(i + 1); j < n; ++j) {
			size_t const ij(ijtok(n, i, j));
			_costs[ij]._v *= _inv_m;
			_allLinks[i][j] = _costs[ij]._v;
			_allLinks[j][i] = _costs[ij]._v;
		}
	}
	//
	_cst = 0;
	for (size_t i(0); i < n; ++i) {
		_cst -= _k[i] * _k[i];
	}
	_cst /= (2 * _m * 2 * _m);
	MY_PRINT(n);
	MY_PRINT(_m);
	MY_PRINT(_cst);
}

void UnipartieInstance::cpCost(DoubleVector &result) const {
	size_t const n(nV());
	result.assign(n * (n - 1) / 2, 0);
	for (size_t i(0); i < n; ++i) {
		for (size_t j(i + 1); j < n; ++j) {
			size_t const ij(ijtok(n, i, j));
			result[ij] = _costs[ij]._v;
		}
	}
}

//void UnipartieInstance::read(std::string const & fileName, std::ostream & stream) {
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

Double UnipartieInstance::computeCost(IndexedList const &v) const {
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

Double UnipartieInstance::computeCost(std::set<size_t> const & v) const {
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

void UnipartieInstance::writeSolution(FractionnarySolution const& bestSolution,
		double lb) const {
	std::ofstream file(
			GetStr("optimal/", problemName(), "_", lb, ".txt").c_str());
	for (auto const & c : bestSolution) {
		for (auto const & edge : costs()) {
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
void UnipartieInstance::getCliquePartitionProblem(
		CliquePartitionProblem & result) const {
	result.nV() = nV();
	result.getEdges() = _edges;
	result.getCosts() = _costs;
	result.getAllLinks() = _allLinks;
}

void UnipartieInstance::branchingWeights(
		FractionnarySolution const & solution,
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
