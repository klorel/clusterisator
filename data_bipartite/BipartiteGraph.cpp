#include "BipartiteGraph.hpp"

BipartiteGraph::BipartiteGraph() :
		_m(0), _inv_m(0), _kR(), _kB(), _edges(), _a() {

}

BipartiteGraph::BipartiteGraph(Edges const & edges) :
		_m(0), _inv_m(0), _kR(), _kB(), _edges(edges), _a() {
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
	_kR.assign(nR(), 0);
	_kB.assign(nB(), 0);
	for (auto const & e : _edges) {
		_kR[e._i] += e._v;
		_kB[e._j] += e._v;
		_a.set(e._i, e._j, e._v);
	}
	_m = (std::accumulate(_a.begin(), _a.end(), 0.0));
	_inv_m = 1.0 / _m;
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
