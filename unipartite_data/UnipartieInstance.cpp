#include "UnipartieInstance.h"

#include "../columnsgeneration/Column.h"
#include "../columnsgeneration/MilpOracle.h"
#include "../columnsgeneration/QpOracle.h"
#include "UnipartiteBinaryDecompositionOracle.h"

UnipartieInstance::UnipartieInstance()
	: _m(0),
	_inv_m(0),
	_edges(),
	_cst(0) {

}

UnipartieInstance::UnipartieInstance(Edges const & edges)
	: _m(0),
	_inv_m(0),
	_edges(edges) {
	build();
}

UnipartieInstance::~UnipartieInstance() {
}
void UnipartieInstance::build() {
	int n(0);
	_m = 0;

	for (auto const & edge : _edges) {
		n = std::max(n, edge._i);
		n = std::max(n, edge._j);
		_m += edge._v;
	}
	n += 1;
	_adjencyGraph.assign(n, AdjencyGraph::value_type());
	_k.assign(n, 0);
	for (auto const & edge : _edges) {
		_k[edge._i] += edge._v;
		_k[edge._j] += edge._v;
	}
	_inv_m = 1.0 / _m;
	_costs.resize(n * (n - 1) / 2);
	_allLinks.assign(nV(), Links());

	for (int i(0); i < n; ++i) {
		for (int j(i + 1); j < n; ++j) {
			int const ij(ijtok(n, i, j));
			_costs[ij] = Edge(i, j, -_k[i] * _k[j] * _inv_m / 2.0);
		}
	}
	for (auto const & edge : _edges) {
		int const ij(ijtok(n, edge._i, edge._j));
		_costs[ij]._v += 1;
		_adjencyGraph[edge._i][edge._j] = 1;
		_adjencyGraph[edge._j][edge._i] = 1;
	}
	for (int i(0); i < n; ++i) {
		for (int j(i + 1); j < n; ++j) {
			int const ij(ijtok(n, i, j));
			_costs[ij]._v *= _inv_m;
			_allLinks[i][j] = _costs[ij]._v;
			_allLinks[j][i] = _costs[ij]._v;
		}
	}
	//
	cst() = 0;
	for (int i(0); i < n; ++i) {
		cst() -= _k[i] * _k[i];
	}
	cst() /= (2 * _m * 2 * _m);
	//	MY_PRINT(n);
	//	MY_PRINT(_m);
	//	MY_PRINT(_cst);
}

Double & UnipartieInstance::cst() {
	return _cst;
}
Double UnipartieInstance::cst() const {
	return _cst;
}
void UnipartieInstance::cpCost(DoubleVector &result) const {
	int const n(nV());
	result.assign(n * (n - 1) / 2, 0);
	for (int i(0); i < n; ++i) {
		for (int j(i + 1); j < n; ++j) {
			int const ij(ijtok(n, i, j));
			result[ij] = _costs[ij]._v;
		}
	}
}

//void UnipartieInstance::read(std::string const & fileName, std::ostream & stream) {
//	std::ifstream file(fileName.c_str());
//	//_a << file;
//	std::string line;
//	std::getline(file, line);
//	int r;
//	int b;
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
	int const n(nV());
	for (auto i : v) {
		for (auto j : v) {
			if (i < j) {
				int const ij(ijtok(n, i, j));
				result += _costs[ij]._v;
			}
		}
	}
	//	double intra(0);
	//	for (auto const & edge : _edges) {
	//		if (v.contains(edge._i) && v.contains(edge._j))
	//			intra += edge._v;
	//	}
	//	double d(0);
	//	for (auto i : v) {
	//		if (v.contains(i))
	//			d += _k[i];
	//	}
	//	double temp(intra * _inv_m - (d * 0.5 * _inv_m)*(d * 0.5 * _inv_m) );

	return result;
}

Double UnipartieInstance::computeCost(IntSet const & v) const {
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
	//	double intra(0);
	//	for (auto const & edge : _edges) {
	//		intra += edge._v;
	//	}
	//	double d(0);
	//	for (auto i : v) {
	//		d += _k[i];
	//	}
	//	double temp(intra * _inv_m - (d * 0.5 * _inv_m) * (d * 0.5 * _inv_m));
	return result;
}
// (xr)/m - (D/2m)²
void UnipartieInstance::update(int id, IndexedList const & v,
	DoubleVector & gradient) const {
	bool const iWasIn(v.contains(id));
	for (auto const & link : _allLinks[id]) {
	  if (iWasIn)
	    gradient[link.first] -= link.second;
	  else
	    gradient[link.first] += link.second;
	}
	//for (auto const & edge : _adjencyGraph[id]) {
	//	int const i(edge.first);
	//	int const j(edge.second);
	//	bool const idIsI(id == i);
	//	bool const idIsJ(id == j);
	//	bool const isI(idIsI ? !v.contains(i) : v.contains(i));
	//	bool const isJ(idIsJ ? !v.contains(j) : v.contains(j));
	//	if (isI && isJ) {
	//		gradient[i] += 2 * _inv_m;
	//		gradient[j] += 2 * _inv_m;
	//	}
	//	else if (isI && !isJ) {
	//		gradient[j] -= 2 * _inv_m;
	//	}
	//	else if (!isI && isJ) {
	//		gradient[i] -= 2 * _inv_m;
	//	}
	//}
	//if (iWasIn)
	//	gradient[id] += 2 * k(id)*k(id)*0.25*_inv_m*_inv_m;
	//else
	//	gradient[id] -= 2 * k(id)*k(id)*0.25*_inv_m*_inv_m;
	//// xId swap
	//for (int j(0); j < nV(); ++j) {
	//	if (j != id) {
	//		bool const jWasIn(v.contains(j));
	//		Double const kikj(k(id)*k(j) * 0.25*_inv_m*_inv_m);
	//		if (jWasIn && iWasIn) {
	//			gradient[id] += 4 * kikj;
	//			gradient[j] += 2 * kikj;
	//		}
	//		else if (jWasIn && !iWasIn) {
	//			gradient[id] -= 4 * kikj;
	//			gradient[j] -= 2 * kikj;
	//		}
	//		else if (!jWasIn && iWasIn) {
	//			gradient[j] -= 2 * kikj;
	//		}
	//		else if (!jWasIn && !iWasIn) {
	//			gradient[j] += 2 * kikj;
	//		}
	//	}
	//}
}

// (xr)/m - (D/2m)²
void UnipartieInstance::gradient(IndexedList const & v,
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
	//for (auto const & edge : _edges) {
	//  int const i(edge._i);
	//  int const j(edge._j);
	//  bool const isI(v.contains(i));
	//  bool const isJ(v.contains(j));
	//  if (isI && isJ) {
	//    result[i] -= _inv_m;
	//    result[j] -= _inv_m;
	//  } else if (isI && !isJ) {
	//    result[j] += _inv_m;
	//  } else if (!isI && isJ) {
	//    result[i] += _inv_m;
	//  }
	//}
	//double factor(std::pow(0.5 * _inv_m, 2));
	//double d(0);
	//for (int i(0); i < nV(); ++i) {
	//	if (v.contains(i)) {
	//		d += k(i);
	//	}
	//}
	//for (int i(0); i < nV(); ++i) {
	//	if (v.contains(i)) {
	//		result[i] -= factor * (k(i) * k(i) + 2 * k(i) * (d - k(i)));
	//	} else {
	//		result[i] += factor * (k(i) * k(i) + 2 * k(i) * (d + k(i)));
	//	}
	//}
	//for (auto const & e : _costs) {
	//	if (v.contains(e._j)) {
	//		result[e._i] += e._v;
	//	}
	//	if (v.contains(e._i)) {
	//		result[e._j] += e._v;
	//	}
	//}
}

void UnipartieInstance::writeSolution(FractionnarySolution const& bestSolution,
	double lb) const {
	std::ofstream file(
		GetStr(problemName(), "_", lb, ".txt").c_str());
	for (auto const & c : bestSolution) {
		for (auto const & edge : costs()) {
			int const i(edge._i);
			int const j(edge._j);
			if (c.first->contains(i) && c.first->contains(j)) {
				file << std::setw(6) << 1 + i;
				file << std::setw(6) << 1 + j;
				file << std::endl;
			}
		}
		//		for (int r(0); r < nR(); ++r) {
		//			for (int b(0); b < nB(); ++b) {
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
	result.cst() = cst();
}

void UnipartieInstance::branchingWeights(FractionnarySolution const & solution,
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
			}
			else if (iFirst || iSecond) {
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
					0.5 * ((int)((t.second.first.size() + t.second.second.size()))),
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
				}
				else if (iR || iB) {
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
						* ((int)((t.second.first.size() + t.second.second.size()))),
						std::make_pair(t.first._i, t.first._j)));
			}
		}
		if (weights.empty())
			std::cout << "weights.empty()" << std::endl;
	}
}
