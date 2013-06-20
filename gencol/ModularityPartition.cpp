#include "ModularityPartition.hpp"


ModularityPartition::ModularityPartition(Graph const & input, size_t k) :
		Partition(input.nbNodes(), k), _input(input){
}
ModularityPartition::ModularityPartition(Graph const &input, Partition const &rhs) :
		Partition(rhs), _input(input) {
}
ModularityPartition::~ModularityPartition() {

}

Double ModularityPartition::computeScore()const{
	Double result(0);
	for(auto const & a : _input.edges()){
		if(label(a.first)==label(a.second)){
			result+= 1- _input.degree(a.first)*_input.degree(a.second)/(2*_input.squareDegree());
		}
	}
	for(size_t n(0); n<_input.nbNodes(); ++n)
		result -= ( _input.degree(n) *_input.degree(n))/(2*_input.squareDegree()); 
	return result;
}