#include "Node.hpp"


Node::Node(BranchAndBound & branchAndBound):_branchAndBound(branchAndBound), _father(NULL){
}
Node::Node(Node const * father):_branchAndBound(father->_branchAndBound), _father(father){
}
Node::~Node(){
}
Double Node::lb()const{
	return _lb;
}
Double Node::ub()const{
	return _ub;
}
Double &Node::lb(){
	return _lb;
}
Double &Node::ub(){
	return _ub;
}


FractionnarySolution & Node::lbSolution(){
	return _lbSolution;
}
Node const * Node::father()const{
	return _father;
}

bool Node::cannot()const{
	return _cannot;
}
size_t Node::r()const{
	return _r;
}
size_t Node::b()const{
	return _b;
}