#include "VnsGenerator.h"

#include "Node.h"
#include "ClusteringProblem.h"

VnsGenerator::VnsGenerator(ClusteringProblem const * input)
    : IOracle(input),
      _allNodes(input->nV(), true),
      _current(input, NULL),
      _best(input, NULL),
      _gradient(input->nV(), 0),
      _wasSwapped(input->nV(), false) {
}
VnsGenerator::~VnsGenerator() {
}

void VnsGenerator::setData(DoubleVector const &dual,
                           DecisionList const &decision) {
  IOracle::setData(dual, decision);
  _current.setDual(dual);
  _best.setDual(dual);

}
void VnsGenerator::swap(int id, Double deltaCost, Double deltaDual) {
  swap(id, _current._v.contains(id), deltaCost, deltaDual);
}
void VnsGenerator::swap(int id, bool wasIn, Double deltaCost,
                        Double deltaDual) {
  _input->update(id, _current._v, _gradient);
  _current.swap(id, deltaCost, deltaDual);
  if (_wasSwapped.contains(id))
    _wasSwapped.erase(id);
  else
    _wasSwapped.insert(id);
}
void VnsGenerator::swap(int id) {

  ASSERT_CHECK(checkGradient());ASSERT_CHECK(check());
  bool const wasIn(_current._v.contains(id));
  Double const deltaCost((wasIn ? -1 : 1) * _gradient[id]);
  Double const deltaDual((wasIn ? -1 : 1) * _current.dual(id));
  swap(id, wasIn, deltaCost, deltaDual);
  ASSERT_CHECK(checkGradient());ASSERT_CHECK(check());
}
void VnsGenerator::shake(int k) {
  _allNodes.fill();
//	_current.check();
  for (int p(0); p < k && !_allNodes.empty(); ++p) {
    int const id(_allNodes.pop_random());
    //		_current.swap(id);
    swap(id);
  }
//	compute();
//	_current.check();
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
  for (auto const & v : _current._v) {
    _current._reducedCost += dual(v);
  }
  _wasSwapped.clear();
}

bool VnsGenerator::checkGradient() const {
  return _input->checkGradient(_current._v, _gradient);

}
bool VnsGenerator::tryMove(int v, Double deltaCost, Double deltaDual) {
  Double const delta(deltaCost + deltaDual);
  bool success(false);
  if (delta > ZERO_REDUCED_COST && !_current._v.contains(v)) {
    swap(v, deltaCost, deltaDual);
    success = true;
  } else if (delta < -ZERO_REDUCED_COST && _current._v.contains(v)) {
    swap(v, -deltaCost, -deltaDual);
    success = true;
  } else {
  }
  return success;
}
bool VnsGenerator::localSearch() {
//	IntVector temp(_input->nV());
  int violations(_current.violation(*_decisions));
  assert(violations == _current.violation(*_decisions));
//	for (int v(0); v < _input->nV(); ++v)
//		_gradient[v] += _current.gradient(v);
  bool stop(false);
  do {
    ASSERT_CHECK(check());
    stop = true;
    for (int v(0); v < _input->nV(); ++v) {
      //			MY_PRINT(_current._reducedCost);
      int const newViolations(violationIf(v));
      if (newViolations == 0 || newViolations < violations) {
        Double const deltaCost(_gradient[v]);
        Double const deltaDual(dual(v));
        if (tryMove(v, deltaCost, deltaDual)) {
          stop = false;
          violations = newViolations;
        }
      }
	  ASSERT_CHECK(checkGradient());
    }
  } while (!stop);
//
  ASSERT_CHECK(violations == _current.violation(*_decisions));
//
  return true;
}

#include "../clustering/Timer.h"

bool VnsGenerator::run(int iteMax, bool stopAtFirst) {
//	_columns.clear();
  int ite(0);
//	Double kMax((Double) std::max(_input->nR(), _input->nB()));
//	Double kMax((Double) std::min(_input->nR(), _input->nB()));
//  Double kMax(_input->nV() > 100 ? 100 : (Double) _input->nV());
	Double kMax((Double) _input->nV() * 0.5 + 1);
//	Double kMax(3);
//	_current.clear();
//	compute();
  initialize();
  Column column(_input);
//	MY_PRINT(ite);
  bool success(false);
  do {
    ++ite;
    //		shake(_input->nV() * 0.5);
    int k(0);
    do {
      ++k;

      shake(k);
      ASSERT_CHECK(check());

      localSearch();
      if (VnsGeneratorSolution::IsBetter(_current, _best, *_decisions)) {
        //				std::cout << "k";
        //				std::cout << std::setw(6) << k;
        //				std::cout << std::setw(6) << kMax;
        //				std::cout << std::endl;
        _wasSwapped.clear();
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
        backTrack();
        ASSERT_CHECK(checkGradient());
        //				_current = _best;
        ASSERT_CHECK(
            std::fabs(_best._reducedCost - _current._reducedCost)
            < 1e-10);
      }
      //			MY_PRINT(_current._reducedCost);
      if (success && stopAtFirst) {
        k = (int) std::ceil(kMax);
      }
    } while (k < kMax);
  } while (ite < iteMax && !success);

  return !_columns.empty();
}
void VnsGenerator::backTrack() {
  //for (auto const & v : _wasSwapped) 
  for (IndexedList::const_iterator vIte(_wasSwapped.begin());
      vIte != _wasSwapped.end(); ++vIte) {
    int const v(*vIte);
    assert(_best._v.contains(v) + _current._v.contains(v) == 1);


    ASSERT_CHECK(checkGradient());ASSERT_CHECK(check());
    bool const wasIn(_current._v.contains(v));
    Double const deltaCost((wasIn ? -1 : 1) * _gradient[v]);
    Double const deltaDual((wasIn ? -1 : 1) * _current.dual(v));
    _input->update(v, _current._v, _gradient);
    _current.swap(v, deltaCost, deltaDual);
    ASSERT_CHECK(checkGradient());ASSERT_CHECK(check());

  }
  _wasSwapped.clear();
}
void VnsGenerator::initialize() {
  _current.clear();
  _best.clear();
  _input->gradient(_current._v, _gradient);
//	_input->gradient(_current._v, _gradient);

//	for (Edge const & edge : _input->edges()) {
//		if (dual(edge._i) + dual(_input->nR() + edge._j) > 1e-6) {
//			if (!_current._v.contains(edge._i))
//				swap(edge._i);
//			if (!_current._v.contains(_input->nR() + edge._j))
//				swap(_input->nR() + edge._j);
//		}
//	}
//	for (Edge const & edge : _cpp->costs()) {
//		if (dual(edge._i) + dual(edge._j) > 1e-6) {
//			if (!_current._v.contains(edge._i))
//				swap(edge._i);
//			if (!_current._v.contains(edge._j))
//				swap(edge._j);
//		}
//	}
  _best = _current;
  _wasSwapped.clear();
//	compute();
}

