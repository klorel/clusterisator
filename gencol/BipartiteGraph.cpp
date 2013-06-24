#include "BipartiteGraph.hpp"

BipartiteGraph::BipartiteGraph(){
}

BipartiteGraph::BipartiteGraph(std::string const & fileName, std::ostream & stream){
	read(fileName, stream);
}

void BipartiteGraph::read(std::string const & fileName, std::ostream & stream){
	std::ifstream file(fileName.c_str());
	_a << file;
	file.close();
	build();
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
size_t BipartiteGraph::nR()const{
	return _a.getN();
}
size_t BipartiteGraph::nB()const{
	return _a.getM();
}
void BipartiteGraph::build(){
	allocate(_a.getN()+_a.getM());
	_kR.assign(nR(), 0);
	_kB.assign(nB(), 0);
	Double  m(0);
	for(size_t i(0); i<nR(); ++i){
		for(size_t j(0); j<nB(); ++j){
			Double const v(_a.get(i,j));
			m+=v;
			_kR[i]+=v;
			_kB[j]+=v;
		}
	}
	for(size_t i(0); i<nR(); ++i){
		for(size_t j(0); j<nB(); ++j){			
			Double const v((_a.get(i,j)-_kR[i]*_kB[j]/m)/m);
			if(std::abs(v)>1e-10){
				operator()(i).push_back(std::make_pair(nR()+j, v));
				operator()(nR()+j).push_back(std::make_pair(i, v));
			}
		}
	}
	finalize();
}