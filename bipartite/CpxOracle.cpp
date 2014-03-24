/*
 * CpxOracle.cpp
 *
 *  Created on: 24 mars 2014
 *      Author: manuel
 */

#include "CpxOracle.h"

CpxOracle::CpxOracle(BipartiteGraph const * input, DoubleVector const * dual,
		DecisionList const * decisions) :
		IOracle(input, dual, decisions), _env(NULL), _prob(NULL), _index() {

}

CpxOracle::~CpxOracle() {
	freeLp();
}

void CpxOracle::applyBranchingRule() {
	if (_rowBuffer.size() != CPXgetnumrows(_env, _prob))
		CPXdelrows(_env, _prob, _rowBuffer.size(),
				CPXgetnumrows(_env, _prob) - 1);
	_decisionBuffer.clear();
	if (!_decisions->empty()) {
		for (Decision const & decision : *_decisions) {
			if (decision.cannot()) {
				// r+b <= 1
				_decisionBuffer.add(1, 'L', decision.name());
				_decisionBuffer.add(decision.r(), 1);
				_decisionBuffer.add(_input->nR() + decision.b(), +1);
			} else {
				// r = b
				_decisionBuffer.add(0, 'E', decision.name());
				_decisionBuffer.add(decision.r(), 1);
				_decisionBuffer.add(_input->nR() + decision.b(), -1);
			}
		}
		_decisionBuffer.add(_env, _prob);
	}
}

// on initialise avec les singletons
void CpxOracle::write(std::string const & fileName) const {
	CPXwriteprob(_env, _prob, fileName.c_str(), "LP");
}
bool CpxOracle::generate() {
	setUpOracle();
//	write();
	CPXsetintparam(_env, CPX_PARAM_SOLUTIONTARGET,
			CPX_SOLUTIONTARGET_OPTIMALGLOBAL);
	CPXmipopt(_env, _prob);

//	checkSolution();
	//	CPXpopulate(_env, _oracle);
	bool result(false);
	_bestReducedCost = ZERO_REDUCED_COST;
	if (CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL
			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_TOL
			|| CPXgetstat(_env, _prob) == CPXMIP_POPULATESOL_LIM
			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED
			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED_TOL) {
		CPXgetobjval(_env, _prob, &_bestReducedCost);
		//_rd-=_cstDual;
//		std::cout << "_bestReducedCost : " << _bestReducedCost << std::endl;
		result = (_bestReducedCost > ZERO_REDUCED_COST);
		if (result) {
			DoubleVector x(CPXgetnumcols(_env, _prob));
			size_t const n(CPXgetsolnpoolnumsolns(_env, _prob));
			//			std::cout << std::setw(4) << n << std::endl;
			Double obj;
			for (size_t i(0); i < n; ++i) {
				CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
				if (obj > ZERO_REDUCED_COST) {
					CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0,
							(int) (x.size() - 1));
					extractAndAddSolution(x, obj);

				}
			}
		}
	} else
		std::cout << "CPXgetstat : " << CPXgetstat(_env, _prob) << std::endl;
	return result;
}

void CpxOracle::freeLp() {
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
	if (_prob != NULL) {
		CPXfreeprob(_env, &_prob);
		_prob = NULL;
	}
}
