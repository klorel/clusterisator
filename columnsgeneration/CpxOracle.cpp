/*
 * CpxOracle.cpp
 *
 *  Created on: 24 mars 2014
 *      Author: manuel
 */

#include "CpxOracle.h"
#include "ClusteringProblem.h"
#include "../mip_solver/CplexSolver.h"

CpxOracle::CpxOracle(ClusteringProblem const * input)
    : IOracle(input),
      _index() {
  _solver = NULL;
}

CpxOracle::~CpxOracle() {
  freeLp();
}

void CpxOracle::initCpx() {
  _solver.reset(new CplexSolver);
  _solver->initLp("CpxOracle");
  initOracle();

}

void CpxOracle::applyBranchingRule() {
//	if (_rowBuffer.size() != CPXgetnumrows(_env, _prob))
//		CPXdelrows(_env, _prob, _rowBuffer.size(), CPXgetnumrows(_env, _prob) - 1);
//	_decisionBuffer.clear();
//	if (!_decisions->empty()) {
//		for (Decision const & decision : *_decisions) {
//			if (decision.cannot()) {
//				// r+b <= 1
//				_decisionBuffer.add(1, 'L', decision.name());
//				_decisionBuffer.add(decision.noeud1(), +1);
//				_decisionBuffer.add(decision.noeud2(), +1);
//			} else {
//				// r = b
//				_decisionBuffer.add(0, 'E', decision.name());
//				_decisionBuffer.add(decision.noeud1(), +1);
//				_decisionBuffer.add(decision.noeud2(), -1);
//			}
//		}
//		_decisionBuffer.add(_env, _prob);
//	}
}


bool CpxOracle::generate() {

  bool result(false);
  // update dual variables
  _solver->chgObj(_index, *_dual);
  _solver->delMipStarts();
  _solver->run();
  _bestReducedCost = ZERO_REDUCED_COST;
//	_solver->
  if (_solver->isOptimal()) {
    _bestReducedCost = _solver->objValue();
    result = (_bestReducedCost > ZERO_REDUCED_COST);
//		MY_PRINT(_bestReducedCost);
//		MY_PRINT(result);
    if (result) {
      DoubleVector x;
//			int const n(CPXgetsolnpoolnumsolns(_env, _prob));
      int const n(1);
//			MY_PRINT(n);
      Double obj;
      for (int i(0); i < n; ++i) {
        _solver->objValue(i, obj);
        if (obj > ZERO_REDUCED_COST || i > 0) {
          _solver->solution(i, x);
          if (i == 0)
            extractAndAddSolution(x, obj);
          else
            extractAndAddSolution(x);
        }
      }
    }
  } else {
//    std::cout << "CPXgetstat : " << CPXgetstat(_env, _prob) << std::endl;
    std::cout << "Exporting lp into error.lp" << std::endl;
    _solver->write("error.lp");
    __SEG_FAULT__;
  }
  return result;
}

void CpxOracle::freeLp() {
}

void CpxOracle::checkSolutions() const {
}

