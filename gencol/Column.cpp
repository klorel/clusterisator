#include "Column.hpp"


Double Column::computeCost()const{
	Double result(0);
	for(auto const & r : first){
		for(auto const & b : second){
			//std::cout << std::setw(6)<<r;
			//std::cout << std::setw(6)<<_input.kR(r);
			//std::cout << std::setw(6)<<b;
			//std::cout << std::setw(6)<<_input.kB(b);
			//std::cout << std::setw(25)<< _input.w(r,b);
			//std::cout << std::endl;
			result += _input.w(r,b);
		}
	}
	return result;
}
Double Column::computeReducedCost(DoubleVector const & dual)const{
	Double result(0);
	for(auto const & r : first){
		for(auto const & b : second){
			result += _input.w(r,b);
		}
	}
	for(auto const & r : first){
		result += dual[r];
	}
	for(auto const & b : second){
		result += dual[_input.nR()+b];
	}
	return result;
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

size_t & Column::id()const{
	return _id;
}

void Column::check(DoubleVector const & dual) const {
	assert(std::fabs(computeReducedCost(dual)-_reducedCost)<1e-10);
	assert(std::fabs(computeCost()-_cost)<1e-10);
}

void Column::print(std::ostream & stream)const{
	std::copy(first.begin(), first.end(), std::ostream_iterator<size_t>(stream << "R : ", " "));
	std::copy(second.begin(), second.end(), std::ostream_iterator<size_t>(stream << "\nB : ", " "));
	stream << std::endl;
}