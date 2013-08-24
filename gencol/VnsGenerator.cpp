#include "VnsGenerator.hpp"
#include "Node.hpp"

VnsGenerator::VnsGenerator(BipartiteGraph const * input,
		DoubleVector const * dual, DecisionList const * decisions) :
		_input(input), _dual(dual), _allNodes(input->nV(), true), _columns(), _current(
				input, dual), _best(input, dual), _decisions(decisions) {

}
VnsGenerator::VnsGenerator(BipartiteGraph const & input,
		DoubleVector const & dual, DecisionList const & decisions) :
		_input(&input), _dual(&dual), _allNodes(input.nV(), true), _columns(), _current(
				&input, &dual), _best(&input, &dual), _decisions(&decisions) {

}
VnsGenerator::~VnsGenerator() {
}
void VnsGenerator::shake(size_t k) {
	_allNodes.fill();
//	_current.check();
	for (size_t p(0); p < k && !_allNodes.empty(); ++p) {
		size_t const id(_allNodes.pop_random());
//		_current.swap(id);
		Double const deltaCost(_current.deltaCost(id));
		Double const deltaDual(_current.deltaDual(id));
		_current.swap(id, deltaCost, deltaDual);
	}
//	compute();
//	_current.check();
}

void VnsGenerator::compute() {
	_current._cost = _current.computeCost();
	_current._reducedCost = _current.computeReducedCost();
	//for(auto const & v : solution._v){
	//	solution._reducedCost += _dual[v];
	//}
	//for(auto const & r : solution._r){
	//	for(auto const & b : solution._b){
	//		solution._cost+=_input->w(r, b);
	//	}
	//}
	//solution._reducedCost += solution._cost;
}

bool VnsGenerator::localSearch() {
	bool success(false);
	bool stop(false);
//	IntVector temp(_input->nV());
	size_t violations(_current.violation(*_decisions));
	do {
		bool rSucess(false);
		// optimization of blue
		for (size_t r(0); r < _input->nR(); ++r) {
			size_t const newViolations(violationIfR(r));
			if (newViolations == 0 || newViolations < violations) {
				Double deltaCost(0);
				for (auto const & b : _current._b)
					deltaCost += _input->w(r, b);
				Double deltaDual(dualR(r));
				Double delta(deltaDual + deltaCost);
				if (delta > ZERO_REDUCED_COST && !_current._r.contains(r)) {
					_current.swap(r);
					_current._cost += deltaCost;
					_current._reducedCost += delta;
					rSucess = true;
					violations = newViolations;
				} else if (delta < -ZERO_REDUCED_COST
						&& _current._r.contains(r)) {
					_current.swap(r);
					_current._cost -= deltaCost;
					_current._reducedCost -= delta;
					rSucess = true;
					violations = newViolations;
				}
			}
		}
		bool bSucess(false);
		for (size_t b(0); b < _input->nB(); ++b) {
			size_t const newViolations(violationIfB(b));
			if (newViolations == 0 || newViolations < violations) {
				Double deltaCost(0);
				for (auto const & r : _current._r)
					deltaCost += _input->w(r, b);
				Double deltaDual(dualB(b));
				Double delta(deltaDual + deltaCost);
				if (delta > ZERO_REDUCED_COST && !_current._b.contains(b)) {
					_current.swap(_input->nR() + b);
					_current._cost += deltaCost;
					_current._reducedCost += delta;
					bSucess = true;
					violations = newViolations;
				} else if (delta < -ZERO_REDUCED_COST
						&& _current._b.contains(b)) {
					_current.swap(_input->nR() + b);
					_current._cost -= deltaCost;
					_current._reducedCost -= delta;
					bSucess = true;
					violations = newViolations;
				}
			}
		}
		stop = !rSucess && !bSucess;
//		_current.check();
//		stop = true;
	} while (!stop);
	//
	assert(violations == _current.violation(*_decisions));
	//
	return success;
}

bool VnsGenerator::run() {
	_columns.clear();
	size_t ite(0);
	size_t iteMax(100);
//	Double kMax((Double) std::max(_input->nR(), _input->nB()));
//	Double kMax((Double) std::min(_input->nR(), _input->nB()));
	Double kMax((Double) _input->nV());
//	_current.clear();
//	compute();
	_current.clear();
//	initialize(current);
	_best = _current;
	Column column(_input);
	do {
		++ite;
		size_t k(0);
//		initialize(current);
		do {
			++k;
			shake(k);
			localSearch();
			if (_current.violation(*_decisions) == 0
					&& _current._reducedCost > ZERO_REDUCED_COST) {
				_current.build(column);
				_columns.insert(column);
			}
			if (VnsGeneratorSolution::IsBetter(_current, _best, *_decisions)) {
				_best = _current;
				k = 0;
			} else {
				_current = _best;
			}
		} while (k < kMax);
//	} while (k < kMax && _columns.empty());

	} while (ite < iteMax && _columns.empty());
	//} while (ite < iteMax && (_columns.empty() || ite >= 0));
//	if (ite > 100)
//		std::cout << "ite " << ite << std::endl;
	//if(ite==iteMax)

	return !_columns.empty();
}

std::set<Column> const & VnsGenerator::columns() const {
	return _columns;
}

void VnsGenerator::sortedColumns(
		std::multimap<Double, Column const *, std::greater<Double>> & result) const {
	result.clear();
	for (auto const & column : _columns) {
		result.insert(std::make_pair(column.reducedCost(), &column));
	}
}
void VnsGenerator::initialize() {
	_current.clear();
	for (size_t i(0); i < _input->nV(); ++i) {
		if (dual(i) > 0)
			_current.swap(i);
	}
	compute();
}

