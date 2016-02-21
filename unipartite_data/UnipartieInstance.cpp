#include "UnipartieInstance.h"
#include "Column.hpp"

#include "MilpOracle.hpp"
#include "QpOracle.hpp"
#include "UnipartiteBinaryDecompositionOracle.hpp"

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
IOracle * UnipartieInstance::newOracle(AvailableOracle oracle,
		DoubleVector const * dual, DecisionList const * decision) const {
	IOracle * result(NULL);
	switch (oracle) {
	case MILP:
		result = new MilpOracle(this, dual, decision);
		break;
	case MIQP:
		result = new QpOracle(this, dual, decision);
		break;
	default:
		result = new UnipartiteBinaryDecompositionOracle(this, dual, decision);
		break;
	}
	return result;
}
