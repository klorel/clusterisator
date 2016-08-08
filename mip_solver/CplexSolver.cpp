/*
 * CplexSolver.cpp
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#include "CplexSolver.h"

CplexSolver::CplexSolver() {
	// TODO Auto-generated constructor stub

}

CplexSolver::~CplexSolver() {
	// TODO Auto-generated destructor stub
}

int CplexSolver::add(RowBuffer  &) {
	return 0;
//	_begin.push_back(nz());
//	if (_name.empty()) {
//		CPXaddrows(env, lp, 0, size(), nz(), _objRhs.data(), _type.data(),
//				_begin.data(), index(), value(), NULL, NULL);
//	} else {
//		//		MY_PRINT(_name.size());
//		//		MY_PRINT(size());
//		assert(
//				(int )_name.size() == size()
//						&& "you should provide a name for each element");
//
//		std::vector<char*> cpxName(_name.size());
//		for (size_t i(0); i < _name.size(); ++i) {
//			cpxName[i] = const_cast<char*>(_name[i].c_str());
//		}
//		CPXaddrows(env, lp, 0, size(), nz(), _objRhs.data(), _type.data(),
//				_begin.data(), index(), value(), NULL, cpxName.data());
//	}
//	_begin.pop_back();
}
int CplexSolver::add(ColumnBuffer  &) {
	return 0;
//	_begin.push_back(nz());
//	if (_name.empty()) {
//		CPXaddcols(env, lp, size(), nz(), _objRhs.data(), _begin.data(),
//				index(), value(), _lower.data(), _upper.data(), NULL);
//	} else {
//		assert(
//				(int )_name.size() == size()
//						&& "you should provide a name for each element");
//
//		std::vector<char*> cpxName(_name.size());
//		for (size_t i(0); i < _name.size(); ++i) {
//			cpxName[i] = const_cast<char*>(_name[i].c_str());
//		}
//		CPXaddcols(env, lp, size(), nz(), _objRhs.data(), _begin.data(),
//				index(), value(), _lower.data(), _upper.data(), cpxName.data());
//	}
//	_begin.pop_back();
//	if (!_only_continous) {
//		std::vector<int> sequence(size());
//		for (int i(0); i < size(); ++i) {
//			sequence[i] = CPXgetnumcols(env, lp) - size() + i;
//		}
//		CPXchgctype(env, lp, size(), sequence.data(), _type.data());
//	}

}

void CplexSolver::write(std::string const & fileName) const {
//	CPXwriteprob(_env, _prob, fileName.c_str(), "LP");
}

void CplexSolver::initLp(std::string const & name) {
//	int err;
//	_env = CPXopenCPLEX(&err);
//	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
//	//	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
//	//	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
//	//	CPXsetintparam(_env, CPX_PARAM_PREPASS, 0);
//	//	CPXsetintparam(_env, CPX_PARAM_CUTPASS, -1);
//	//	CPXsetintparam(_env, CPX_PARAM_VARSEL, 4);
//	CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY, 2);

}
void CplexSolver::freeLp() {
//	if (_env != NULL) {
//		CPXcloseCPLEX(&_env);
//		_env = NULL;
//	}
//	if (_prob != NULL) {
//		CPXfreeprob(_env, &_prob);
//		_prob = NULL;
//	}
}

void CplexSolver::chgObj(IntVector const & indexe,
		DoubleVector const & values) {

}

size_t CplexSolver::numMipStarts() {
	return 0;
}
int CplexSolver::delMipStarts() {
	return 0;
}

bool CplexSolver::isOptimal() const {
	return true;
//	return CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL
//			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_TOL
//			|| CPXgetstat(_env, _prob) == CPXMIP_POPULATESOL_LIM
//			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED
//			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED_TOL;
}


void CplexSolver::applyBranchingRule(){
//	if (_rowBuffer.size() != CPXgetnumrows(_env, _prob))
//		CPXdelrows(_env, _prob, _rowBuffer.size(),
//				CPXgetnumrows(_env, _prob) - 1);
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

void CplexSolver::checkSolutions() const{
//
//	Double obj;
//	DoubleVector x(CPXgetnumcols(_env, _prob));
//	size_t const n(CPXgetsolnpoolnumsolns(_env, _prob));
//	for (size_t i(0); i < n; ++i) {
//		CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
//		CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0,
//				(int) (x.size() - 1));
//		Column column(_input);
//		for (size_t v(0); v < _input->nV(); ++v) {
//			if (x[v] > 0.5) {
//				column.insert(v);
//			}
//		}
//		column.cost() = column.computeCost();
//		column.reducedCost() = obj;
//		std::cout << "ID";
//		std::cout << std::setw(6) << i;
//		std::cout << std::setw(20) << column.cost();
//		std::cout << std::setw(20) << obj;
//		std::cout << std::endl;
//		ASSERT_CHECK(column.check(*_dual));
//		for (Decision const & decision : *_decisions) {
//			if (column.violation(decision) > 0) {
//				decision.print(
//						std::cout << "violation in MipGenerator::generate() ");
//				std::cout << std::endl;
//				column.print();
//			}
//		}
//	}
}
bool CplexSolver::generate()
{
	return true;
//	setUpOracle();
////	write();
//	CPXsetintparam(_env, CPX_PARAM_SOLUTIONTARGET,
//	CPX_SOLUTIONTARGET_OPTIMALGLOBAL);
//	CPXmipopt(_env, _prob);
////	checkSolutions();
////	checkSolution();
//	//	CPXpopulate(_env, _oracle);
//	bool result(false);
//	_bestReducedCost = ZERO_REDUCED_COST;
//	if (CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL
//			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_TOL
//			|| CPXgetstat(_env, _prob) == CPXMIP_POPULATESOL_LIM
//			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED
//			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED_TOL) {
//		CPXgetobjval(_env, _prob, &_bestReducedCost);
//		//_rd-=_cstDual;
//
//		result = (_bestReducedCost > ZERO_REDUCED_COST);
//
////		MY_PRINT(_bestReducedCost);
////		MY_PRINT(result);
//		if (result) {
//			DoubleVector x(CPXgetnumcols(_env, _prob));
////			size_t const n(CPXgetsolnpoolnumsolns(_env, _prob));
//			size_t const n(1);
////			MY_PRINT(n);
//			Double obj;
//			for (size_t i(0); i < n; ++i) {
//				CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
//				if (obj > ZERO_REDUCED_COST || i > 0) {
//					CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0,
//							(int) (x.size() - 1));
//					if (i == 0)
//						extractAndAddSolution(x, obj);
//					else
//						extractAndAddSolution(x);
//				}
//			}
//		}
//	} else {
//		std::cout << "CPXgetstat : " << CPXgetstat(_env, _prob) << std::endl;
//		std::cout << "Exporting lp into error.lp" << std::endl;
//		write("error.lp");
//		__SEG_FAULT__;
//	}
//	return result;
}
