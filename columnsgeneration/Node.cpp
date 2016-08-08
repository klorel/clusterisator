#include "Node.h"

Node::Node(BranchAndBound const& branchAndBound) :
		_branchAndBound(&branchAndBound), _father(NULL), _ub(1e50), _lb(-1e50), _decision() {
	_isInteger = false;
	_id = 0;
}

Node::Node(Node const * father, bool cannot, int noeud1, int noeud2) :
		_branchAndBound(father->_branchAndBound), _father(father), _ub(+1e50), _lb(
				-1e50), _decision(noeud1, noeud2, cannot) {
	_isInteger = false;
	_id = 0;
}
Node::~Node() {
}
Double Node::lb() const {
	return _lb;
}
Double Node::ub() const {
	return _ub;
}
Double &Node::lb() {
	return _lb;
}
Double &Node::ub() {
	return _ub;
}

int Node::id() const {
	return _id;
}
int & Node::id() {
	return _id;
}
FractionnarySolution & Node::lbSolution() {
	return _lbSolution;
}
FractionnarySolution const & Node::lbSolution() const {
	return _lbSolution;
}
Node const * Node::father() const {
	return _father;
}
bool & Node::isInteger() {
	return _isInteger;
}
bool Node::isInteger() const {
	return _isInteger;
}

void Node::decisions(DecisionList& result) const {
	if (!isRoot()) {
		result.push_front(_decision);
		father()->decisions(result);
	}

}
void Node::decisions(DecisionSet&result) const {
	if (!isRoot()) {
		result.insert(_decision);
		father()->decisions(result);
	}

}
Decision const & Node::decision() const {
	return _decision;
}

bool Node::isRoot() const {
	return _father == NULL;
}
