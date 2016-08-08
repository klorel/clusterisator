#include "VnsGeneratorSolution.h"

#include "Node.h"

#include "ClusteringProblem.h"

VnsGeneratorSolution::VnsGeneratorSolution(ClusteringProblem const * input,
		DoubleVector const * dual) :
		_input(input), _dual(dual), _v(_input->nV()), _cost(0), _reducedCost(0) {

}
void VnsGeneratorSolution::clear() {
	_cost = 0;
	_reducedCost = 0;
	_v.clear();

}
VnsGeneratorSolution & VnsGeneratorSolution::operator=(
		VnsGeneratorSolution const & rhs) {
	if (&rhs != this) {
		_v = rhs._v;
		_cost = rhs._cost;
		_reducedCost = rhs._reducedCost;
	}
	return *this;
}
VnsGeneratorSolution::~VnsGeneratorSolution() {
}

void VnsGeneratorSolution::swap(int id) {
	if (_v.contains(id)) {
		_v.erase(id);
	} else {
		_v.insert(id);
	}
}
void VnsGeneratorSolution::swap(int id, Double deltaCost, Double deltaDual) {
	swap(id);
	_cost += deltaCost;
	_reducedCost += (deltaCost + deltaDual);
}
Double VnsGeneratorSolution::computeCost() const {
	return _input->computeCost(_v);
}
Double VnsGeneratorSolution::computeReducedCost() const {
	Double result(computeCost());
	for (auto const & v : _v) {
		result += dual(v);
	}
	return result;
}

void VnsGeneratorSolution::build(Column & column) {
	column.v().clear();
	for (auto const v : _v) {
		column.insert(v);
	}

	column.cost() = _cost;
	column.reducedCost() = _reducedCost;
	ASSERT_CHECK(check());ASSERT_CHECK(column.check(*_dual));
//	column.print();
}

int VnsGeneratorSolution::violation(DecisionList const & decisions) const {
	int result(0);
	for (auto const & decision : decisions) {
		result += decision.violation(_v.contains(decision.noeud1()),
				_v.contains(decision.noeud2()));
	}
	return result;
}
bool VnsGeneratorSolution::IsBetter(VnsGeneratorSolution const & p,
		VnsGeneratorSolution const & q, DecisionList const & decisions) {
	size_t const pViolation(p.violation(decisions));
	size_t const qViolation(q.violation(decisions));
	if (pViolation == qViolation) {
		return p._reducedCost > q._reducedCost + ZERO_REDUCED_COST;
	} else {
		return pViolation < qViolation;
	}
}
//bool VnsGeneratorSolution::IsBetter(VnsGeneratorSolution const & p,
//		VnsGeneratorSolution const & q) {
//	return p._reducedCost > q._reducedCost + ZERO_REDUCED_COST;
//}

bool VnsGeneratorSolution::check() const {
	if (std::fabs(computeCost() - _cost) > 1e-6) {
		std::cout << "wrong cost ";
		std::cout << std::setw(15) << std::setprecision(10) << _cost;
		std::cout << std::setw(15) << std::setprecision(10) << computeCost();
		std::cout << std::endl;
		return false;
	}
	if (std::fabs(computeReducedCost() - _reducedCost) > 1e-6) {
		std::cout << "wrong reduced cost ";
		std::cout << std::setw(15) << std::setprecision(10) << _reducedCost;
		std::cout << std::setw(15) << std::setprecision(10)
				<< computeReducedCost();
		std::cout << std::endl;
		return false;
	}
	return true;
}
void VnsGeneratorSolution::setDual(DoubleVector const & v) {
	_dual = &v;
}

