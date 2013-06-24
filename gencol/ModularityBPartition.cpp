#include "ModularityBPartition.hpp"


ModularityBPartition::ModularityBPartition(BipartiteGraph const & input, size_t k) :
	Partition(input.nbNodes(), k), _input(input){
}
ModularityBPartition::ModularityBPartition(BipartiteGraph const &input, Partition const &rhs) :
	Partition(rhs), _input(input) {
}
ModularityBPartition::~ModularityBPartition() {

}

Double ModularityBPartition::computeScore()const{
	Double result(0);
	for(auto const & a : _input.edges()){
		if(label(a._i)==label(a._j)){
			result+= a._v ;
		}
	}
	return result;
}

BipartiteGraph const & ModularityBPartition::input()const{
	return _input;
}