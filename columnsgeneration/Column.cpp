#include "Column.h"

Column::Column(ClusteringProblem const & input) :
		_input(&input), _cost(0), _reducedCost(0), _id(0) {
}
Column::Column(ClusteringProblem const * input) :
		_input(input), _cost(0), _reducedCost(0), _id(0) {
}
Column::~Column() {
}

Double Column::computeCost() const {
	Double result(_input->computeCost(_v));
	return result;
}
Double Column::computeReducedCost(DoubleVector const & dual) const {
	Double result(computeCost());
	for (auto const & v : _v) {
		result += dual[v];
	}
	return result;
}
int Column::size() const {
	return (int) _v.size();
}

Double & Column::cost() {
	return _cost;
}
Double const & Column::cost() const {
	return _cost;
}
Double & Column::reducedCost() {
	return _reducedCost;
}
Double const & Column::reducedCost() const {
	return _reducedCost;
}
int & Column::id() const {
	return _id;
}

bool Column::check(DoubleVector const & dual) const {
	if (std::fabs(computeCost() - _cost) > 1e-6) {
		std::cout << "wrong cost ";
		std::cout << _cost << ", should be ";
		std::cout << computeCost() << std::endl;
		return false;
	}
	if (std::fabs(computeReducedCost(dual) - _reducedCost) > 1e-6) {
		std::cout << "wrong reduced cost ";
		std::cout << _reducedCost << ", should be ";
		std::cout << computeReducedCost(dual) << std::endl;
		return false;
	}
	return true;

}

bool Column::check() const {
	if (std::fabs(computeCost() - _cost) > 1e-6) {
		std::cout << "wrong cost " << std::endl;
		std::cout << _cost << std::endl;
		std::cout << computeCost() << std::endl;
//		assert(false && "WRONG COST");
		return false;
	}
	return true;

}

void Column::print(std::ostream & stream) const {
	std::copy(_v.begin(), _v.end(),
			std::ostream_iterator<size_t>(stream << "V : ", " "));
	stream << std::endl;
}
IntSet & Column::v() {
	return _v;
}
IntSet const & Column::v() const {
	return _v;
}
bool Column::operator<(Column const & rhs) const {
	return _v < rhs._v;
}

int Column::violation(DecisionList const & decisions) const {
	int result(0);
	for (auto const & decision : decisions) {
		result += violation(decision);
	}
	return result;

}
int Column::violation(Decision const & decision) const {
	bool const isR(contains(decision.noeud1()));
	bool const isB(contains(decision.noeud2()));
	return decision.violation(isR, isB);
}

bool Column::insert(int v) {
	return _v.insert(v).second;

}
void Column::erase(int v) {
	_v.erase(v);

}
bool Column::contains(int v) const {
	return _v.find(v) != _v.end();
}
