#include "VnsGenerator.hpp"
#include "Node.hpp"

VnsGenerator::VnsGenerator(BipartiteGraph const * input,
		DoubleVector const * dual, DecisionList const * decisions) :
		IOracle(input, dual, decisions), _allNodes(input->nV(), true), _current(
				input, dual), _best(input, dual) {

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
	ASSERT_CHECK(check());
}
bool VnsGenerator::check(bool alsoDecision) const {
	if (_current.violation(*_decisions) == 0) {
		if (!_current.check()) {
			std::cout << "_current.check() is false" << std::endl;
			return false;
		}
	}
	if (alsoDecision && _current.violation(*_decisions) != 0) {
		std::cout << "_current.violation(*_decisions) != 0" << std::endl;
		return false;;
	}
	return true;
}
void VnsGenerator::compute() {
	_current._cost = _current.computeCost();
//	_current._reducedCost = _current.computeReducedCost();
	for (auto const & v : _current._v) {
		_current._reducedCost += dual(v);
	}
	//for(auto const & r : solution._r){
	//	for(auto const & b : solution._b){
	//		solution._cost+=_input->w(r, b);
	//	}
	//}
	//solution._reducedCost += solution._cost;
}

bool VnsGenerator::localSearch() {
	bool stop(false);
//	IntVector temp(_input->nV());
	size_t violations(_current.violation(*_decisions));
	assert(violations == _current.violation(*_decisions));
	do {
		ASSERT_CHECK(check());
		stop = true;
		for (size_t v(0); v < _input->nV(); ++v) {
			size_t const newViolations(violationIf(v));
			if (newViolations == 0 || newViolations < violations) {
				Double const deltaCost(_current.gradient(v));
				Double const deltaDual(dual(v));
				Double const delta(deltaDual + deltaCost);
				if (delta > ZERO_REDUCED_COST && !_current._v.contains(v)) {
					_current.swap(v, deltaCost, deltaDual);
					stop = false;
					violations = newViolations;
				} else if (delta < -ZERO_REDUCED_COST
						&& _current._v.contains(v)) {
					_current.swap(v, -deltaCost, -deltaDual);
					stop = false;
					violations = newViolations;
				}
			}
		}
	} while (!stop);
	//
	ASSERT_CHECK(violations == _current.violation(*_decisions));

	//
	return true;
}

bool VnsGenerator::run(size_t iteMax, bool stopAtFirst) {
	_columns.clear();
	size_t ite(0);
//	Double kMax((Double) std::max(_input->nR(), _input->nB()));
//	Double kMax((Double) std::min(_input->nR(), _input->nB()));
	Double kMax(_input->nV() > 50 ? 50 : (Double) _input->nV());
//	Double kMax((Double) _input->nV() * 0.5 + 1);
//	Double kMax(3);
//	_current.clear();
//	compute();
	_current.clear();
	initialize();
	_best = _current;
	Column column(_input);
	bool success(false);
	do {
		++ite;
		size_t k(0);
		do {
			++k;
			shake(k);
			ASSERT_CHECK(check());
			localSearch();

			if (!stopAtFirst && _current.violation(*_decisions) == 0
					&& _current._reducedCost > ZERO_REDUCED_COST) {
				ASSERT_CHECK(check(true));
				_current.build(column);
				_columns.insert(column);
				success = true;
			}
			if (VnsGeneratorSolution::IsBetter(_current, _best, *_decisions)) {
				_best = _current;
				k = 0;
				if (_current.violation(*_decisions) == 0
						&& _current._reducedCost > ZERO_REDUCED_COST) {
					ASSERT_CHECK(check(true));
					_current.build(column);
					_columns.insert(column);
					success = true;
				}
			} else {
				_current = _best;
			}
			if (success && stopAtFirst)
				k = std::ceil(kMax);
		} while (k < kMax);
	} while (ite < iteMax && !success);

	return !_columns.empty();
}

void VnsGenerator::initialize() {
	_current.clear();
//	for (size_t i(0); i < _input->nV(); ++i) {
//		if (dual(i) > 0)
//			_current.swap(i);
//	}
//	for (Edge const & edge : _input->edges()) {
//		if (dual(edge._i) + dual(_input->nR() + edge._j) > 1e-6) {
//			if (!_current._v.contains(edge._i))
//				_current.swap(edge._i);
//			if (!_current._v.contains(_input->nR() + edge._j))
//				_current.swap(_input->nR() + edge._j);
//		}
//	}
	compute();
}

