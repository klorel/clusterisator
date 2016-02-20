#include "BipartiteGraph.hpp"
#include "MilpOracle.hpp"
#include "QpOracle.hpp"

#include "BinaryDecompositionOracle.hpp"

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
	size_t R(0);
	size_t B(0);
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
	_allLinks.assign(nV(), std::map<size_t, double>());
	_costs.reserve(nR() * nB());

	for (size_t r(0); r < nR(); ++r) {
		for (size_t b(0); b < nB(); ++b) {
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
	size_t const n(nV());
	result.assign(n * (n - 1) / 2, 0);
	for (size_t r(0); r < nR(); ++r) {
		for (size_t b(0); b < nB(); ++b) {
			double const value(w(r, b));
			if (value != 0) {
				size_t const ij(ijtok(n, r, nR() + b));
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

Double BipartiteGraph::computeCost(std::set<size_t> const & v) const {
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

IOracle * BipartiteGraph::newOracle(AvailableOracle oracle,
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
		result = new BinaryDecompositionOracle(this, dual, decision);
		break;
	}
	return result;
}
