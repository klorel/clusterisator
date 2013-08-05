#include "Column.hpp"


void Column::computeCost(){
	_cost = 0;
	for(auto const & r : first){
		for(auto const & b : second){
			//std::cout << std::setw(6)<<r;
			//std::cout << std::setw(6)<<_input.kR(r);
			//std::cout << std::setw(6)<<b;
			//std::cout << std::setw(6)<<_input.kB(b);
			//std::cout << std::setw(25)<< _input.w(r,b);
			//std::cout << std::endl;
			_cost += _input.w(r,b);
		}
	}
}
Column::Column(BipartiteGraph const & input):_input(input){
}
Column::~Column(){
}


size_t Column::size()const{
	return first.size()+second.size();
}

Double & Column::cost(){
	return _cost;
}
Double const & Column::cost()const {
	return _cost;
}
Double & Column::reducedCost(){
	return _reducedCost;
}
Double const & Column::reducedCost()const {
	return _reducedCost;
}
void Column::addElement(size_t i){
	if(i>_input.nR())
		second.insert(i-_input.nR()-1);
	else
		first.insert(i-1);
}