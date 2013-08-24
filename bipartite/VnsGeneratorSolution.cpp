#include "VnsGeneratorSolution.hpp"
#include "Node.hpp"

VnsGeneratorSolution::VnsGeneratorSolution(BipartiteGraph const * input,
		DoubleVector const * dual) :
		_input(input), _dual(dual), _v(_input->nV()), _r(_input->nR()), _b(
				_input->nB()), _cost(0), _reducedCost(0) {

}
VnsGeneratorSolution::VnsGeneratorSolution(BipartiteGraph const & input,
		DoubleVector const & dual) :
		_input(&input), _dual(&dual), _v(_input->nV()), _r(_input->nR()), _b(
				_input->nB()), _cost(0), _reducedCost(0) {

}
void VnsGeneratorSolution::clear() {
	_cost = 0;
	_reducedCost = 0;
	_v.clear();
	_r.clear();
	_b.clear();

}
VnsGeneratorSolution & VnsGeneratorSolution::operator=(
		VnsGeneratorSolution const & rhs) {
	if (&rhs != this) {
		_v = rhs._v;
		_r = rhs._r;
		_b = rhs._b;
		_cost = rhs._cost;
		_reducedCost = rhs._reducedCost;
	}
	return *this;
}
VnsGeneratorSolution::~VnsGeneratorSolution() {
}

void VnsGeneratorSolution::swap(size_t id) {
	bool const isR(id < _input->nR());
	if (_v.contains(id)) {
		_v.erase(id);
		if (isR)
			_r.erase(id);
		else
			_b.erase(id - _input->nR());
	} else {
		_v.insert(id);
		if (isR)
			_r.insert(id);
		else
			_b.insert(id - _input->nR());
	}
}
void VnsGeneratorSolution::swap(size_t id, Double deltaCost, Double deltaDual) {
	swap(id);
	_cost += deltaCost;
	_reducedCost += (deltaCost + deltaDual);
}
Double VnsGeneratorSolution::computeCost() const {
	Double result(0);
	for (auto const & r : _r) {
		for (auto const & b : _b) {
			result += _input->w(r, b);
		}
	}
	return result;
}
Double VnsGeneratorSolution::computeReducedCost() const {
	Double result(computeCost());
	for (auto const & r : _r) {
		result += dualR(r);
	}
	for (auto const & b : _b) {
		result += dualB(b);
	}
	return result;
}

void VnsGeneratorSolution::build(Column & column) {

	column.r().clear();
	column.b().clear();
	for (auto const r : _r) {
		column.r().insert(r);
	}
	for (auto const b : _b) {
		column.b().insert(b);
	}
	column.cost() = _cost;
	column.reducedCost() = _reducedCost;
//	column.print();
}

size_t VnsGeneratorSolution::violation(DecisionList const & decisions) const {
	size_t result(0);
	for (auto const & decision : decisions) {
		result += decision.violation(_r.contains(decision.r()),
				_b.contains(decision.b()));
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

void VnsGeneratorSolution::check() const {
	if (std::fabs(computeCost() - _cost) > 1e-10) {
		std::cout << "wrong cost " << std::endl;
		std::cout << _cost << std::endl;
		std::cout << computeCost() << std::endl;
		assert(false && "WRONG COST");
	}
	if (std::fabs(computeReducedCost() - _reducedCost) > 1e-10) {
		std::cout << "wrong reduced cost " << std::endl;
		std::cout << _reducedCost << std::endl;
		std::cout << computeReducedCost() << std::endl;
		assert(false && "WRONG REDUCED COST");
	}

}
