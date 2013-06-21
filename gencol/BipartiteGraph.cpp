#include "BipartiteGraph.hpp"


BipartiteGraph::BipartiteGraph(std::string const & fileName, std::ostream & stream){
	read(fileName, stream);
}

void BipartiteGraph::read(std::string const & fileName, std::ostream & stream){
	std::ifstream file(fileName.c_str());
	_a << file;
	file.close();
}
BipartiteGraph::~BipartiteGraph(){
}


void BipartiteGraph::exportAmpl(std::string const & fileName)const{
	std::ofstream file(fileName.c_str());
	file << "param nR := "<<_a.getN()<<";"<<std::endl;
	file << "param nB := "<<_a.getM()<<";"<<std::endl;
	file << "set E  := "<<std::endl;
	for(size_t i(0); i<_a.getN(); ++i){
		for(size_t j(0); j<_a.getM(); ++j){
			Double const v(_a.get(i,j));
			if( v != 0){
				file << std::setw(6)<<i+1;
				file << std::setw(6)<<j+1;
				//file << std::setw(6)<<v;
				file << std::endl;
			}
		}
	}
	file << ";"<<std::endl;
	file.close();

}