#include "BipartiteGraph.hpp"

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
	size_t r(0);
	size_t b(0);
//	std::cout << _edges << std::endl;
	for (auto const & e : _edges) {
		r = std::max(r, e._i);
		b = std::max(b, e._j);
	}
	_a.allocate(r + 1, b + 1);
	_k.assign(r + 1 + b + 1, 0);
	for (auto const & e : _edges) {
		_a.set(e._i, e._j, e._v);
		_k[e._i] += e._v;
		_k[r + 1 + e._j] += e._v;
	}
	_m = (std::accumulate(_a.begin(), _a.end(), 0.0));
	_inv_m = 1.0 / _m;
	_allLinks.assign(nV(), std::map<size_t, double>());
	for (size_t i(0); i < nR(); ++i) {
		for (size_t j(0); j < nB(); ++j) {
			double const value(w(i, j));
			if (value != 0) {
				_allLinks[i][nR() + j] = value;
				_allLinks[nR() + j][i] = value;
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

std::string BipartiteGraph::name() const {
	return "";
}
