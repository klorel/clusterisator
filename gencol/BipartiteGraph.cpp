#include "BipartiteGraph.hpp"

BipartiteGraph::BipartiteGraph() :
		_kR(), _kB(), _edges(), _a() {

	_m = 0;
	_inv_m = 0;

}

BipartiteGraph::BipartiteGraph(std::string const & fileName,
		std::ostream & stream) {
	read(fileName, stream);
}

void BipartiteGraph::read(std::string const & fileName, std::ostream & stream) {
	std::ifstream file(fileName.c_str());
	//_a << file;
	std::string line;
	std::getline(file, line);
	size_t r;
	size_t b;
	{
		std::stringstream buffer(line);
		buffer >> r;
		buffer >> b;
	}
	_a.allocate(r, b);
	while (std::getline(file, line)) {
		std::stringstream buffer(line);
		buffer >> r;
		buffer >> b;
		_a.get(r - 1, b - 1) = 1;
	}
	file.close();
	build();
}
BipartiteGraph::~BipartiteGraph() {
}

void BipartiteGraph::exportAmpl(std::string const & fileName) const {
	std::ofstream file(fileName.c_str());
	file << "param nR := " << _a.getN() << ";" << std::endl;
	file << "param nB := " << _a.getM() << ";" << std::endl;
	file << "set E  := " << std::endl;
	for (size_t i(0); i < _a.getN(); ++i) {
		for (size_t j(0); j < _a.getM(); ++j) {
			Double const v(_a.get(i, j));
			if (v != 0) {
				file << std::setw(6) << i + 1;
				file << std::setw(6) << j + 1;
				//file << std::setw(6)<<v;
				file << std::endl;
			}
		}
	}
	file << ";" << std::endl;
	file.close();

}
void BipartiteGraph::build() {
	//allocate(_a.getN()+_a.getM());
	_kR.assign(nR(), 0);
	_kB.assign(nB(), 0);
	//Double  m(0);
	_m = (std::accumulate(_a.begin(), _a.end(), 0.0));
	for (size_t r(0); r < nR(); ++r) {
		for (size_t b(0); b < nB(); ++b) {
			Double const v(_a.get(r, b));
			if (v != 0) {
				//std::cout << std::setw(6)<<r+1;
				//std::cout << std::setw(6)<<b+1;
				//std::cout << std::endl;
				_kR[r] += v;
				_kB[b] += v;
				_edges.push_back(Edge(r, b, v));
			}

		}
	}
	//for(size_t i(0); i<nR(); ++i){
	//	for(size_t j(0); j<nB(); ++j){			
	//		Double const v((_a.get(i,j)-_kR[i]*_kB[j]/m)/m);
	//		if(std::abs(v)>1e-10){
	//			operator()(i).push_back(std::make_pair(nR()+j, v));
	//			operator()(nR()+j).push_back(std::make_pair(i, v));
	//		}
	//	}
	//}
	//finalize();
	_inv_m = 1.0 / _m;
}
