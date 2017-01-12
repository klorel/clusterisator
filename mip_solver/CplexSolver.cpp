/*
 * CplexSolver.cpp
 *
 *  Created on: 8 ao�t 2016
 *      Author: manuruiz
 */

#include "CplexSolver.h"
#include "LpBuffer.h"
#include "Decision.h"

CplexSolver::CplexSolver() :ILpSolver(){
	_prob = NULL;
	_env = NULL;

}

CplexSolver::~CplexSolver() {
	// TODO Auto-generated destructor stub
}
#ifndef __LAZY_CPLEX__
#include <cplex.h>
void CplexSolver::add(RowBuffer & buffer) {
	buffer.add_last_begin();
	if (buffer.name().empty()) {
		CPXaddrows(_env, _prob, 0, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.type(), buffer.begin(), buffer.index(), buffer.value(),
		NULL,
		NULL);
	} else {
		//		MY_PRINT(_name.size());
		//		MY_PRINT(size());
		assert((int )buffer.name().size() == buffer.size() && "you should provide a name for each element");

		std::vector<char*> cpxName(buffer.name().size());
		for (int i(0); i < buffer.name().size(); ++i) {
			cpxName[i] = const_cast<char*>(buffer.name()[i].c_str());
		}
		CPXaddrows(_env, _prob, 0, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.type(), buffer.begin(), buffer.index(), buffer.value(),
		NULL, cpxName.data());
	}
	buffer.rem_last_begin();
}
void CplexSolver::add(ColumnBuffer & buffer) {
	buffer.add_last_begin();
	if (buffer.name().empty()) {
		CPXaddcols(_env, _prob, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.begin(), buffer.index(), buffer.value(), buffer.lower(), buffer.upper(), NULL);
	} else {
		assert((int )buffer.name().size() == buffer.size() && "you should provide a name for each element");

		std::vector<char*> cpxName(buffer.name().size());
		for (int i(0); i < buffer.name().size(); ++i) {
			cpxName[i] = const_cast<char*>(buffer.name()[i].c_str());
		}
		CPXaddcols(_env, _prob, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.begin(), buffer.index(), buffer.value(), buffer.lower(), buffer.upper(), cpxName.data());
	}
	buffer.rem_last_begin();
	if (!buffer.only_continous()) {
		_is_mip = true;
		std::vector<int> sequence(buffer.size());
		for (int i(0); i < buffer.size(); ++i) {
			sequence[i] = CPXgetnumcols(_env, _prob) - buffer.size() + i;
		}
		CPXchgctype(_env, _prob, buffer.size(), sequence.data(), buffer.type());
	}
}

void CplexSolver::write(std::string const & fileName) const {
	CPXchgobjsen(_env, _prob, _is_minimize ? CPX_MIN : CPX_MAX);
	CPXwriteprob(_env, _prob, fileName.c_str(), "LP");
}

void CplexSolver::initLp(std::string const & name) {
	int err;
	_env = CPXopenCPLEX(&err);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	//CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
//	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
	//CPXsetintparam(_env, CPX_PARAM_PREPASS, 0);
	//	CPXsetintparam(_env, CPX_PARAM_CUTPASS, -1);
	//	CPXsetintparam(_env, CPX_PARAM_VARSEL, 4);
	CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY, 2);
	_prob = CPXcreateprob(_env, &err, name.c_str());

}
void CplexSolver::freeLp() {
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
	if (_prob != NULL) {
		CPXfreeprob(_env, &_prob);
		_prob = NULL;
	}
}

void CplexSolver::chgObj(IntVector const & indexe, DoubleVector const & values) {
	CPXchgobj(_env, _prob, (int) indexe.size(), indexe.data(), values.data());
}

void CplexSolver::delMipStarts() {
	if (CPXgetnummipstarts(_env, _prob) > 1)
		CPXdelmipstarts(_env, _prob, 0, CPXgetnummipstarts(_env, _prob) - 1);
}

void CplexSolver::delRows(int first, int last) {
	CPXdelrows(_env, _prob, first, last);
}
bool CplexSolver::isOptimal() const {
	return CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL || CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_TOL || CPXgetstat(_env, _prob) == CPXMIP_POPULATESOL_LIM || CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED
			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED_TOL;
}

void CplexSolver::applyBranchingRule(DecisionList const & decisions, RowBuffer & rowBuffer, RowBuffer & decisionBuffer) {
	if (rowBuffer.size() != CPXgetnumrows(_env, _prob))
		CPXdelrows(_env, _prob, rowBuffer.size(), CPXgetnumrows(_env, _prob) - 1);
	decisionBuffer.clear();
	if (!decisions.empty()) {
		for (Decision const & decision : decisions) {
			if (decision.cannot()) {
				// r+b <= 1
				decisionBuffer.add(1, 'L', decision.name());
				decisionBuffer.add(decision.noeud1(), +1);
				decisionBuffer.add(decision.noeud2(), +1);
			} else {
				// r = b
				decisionBuffer.add(0, 'E', decision.name());
				decisionBuffer.add(decision.noeud1(), +1);
				decisionBuffer.add(decision.noeud2(), -1);
			}
		}
		decisionBuffer.add(_env, _prob);
	}
}

void CplexSolver::checkSolutions() const {
//
//	Double obj;
//	DoubleVector x(CPXgetnumcols(_env, _prob));
//	int const n(CPXgetsolnpoolnumsolns(_env, _prob));
//	for (int i(0); i < n; ++i) {
//		CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
//		CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0,
//				(int) (x.size() - 1));
//		Column column(_input);
//		for (int v(0); v < _input->nV(); ++v) {
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
bool CplexSolver::generate() {
	return false;
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
////			int const n(CPXgetsolnpoolnumsolns(_env, _prob));
//			int const n(1);
////			MY_PRINT(n);
//			Double obj;
//			for (int i(0); i < n; ++i) {
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

void CplexSolver::setUpOracle() {

}
void CplexSolver::initOracle() {

}
double CplexSolver::objValue() const {
	double result(0);
	CPXgetobjval(_env, _prob, &result);
	return result;
}

void CplexSolver::objValue(int i, Double & obj) const {
	CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);

}
void CplexSolver::solution(int i, DoubleVector & x) const {
	x.resize(ncols());
	CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0, (int) (x.size() - 1));
}

void CplexSolver::primal(DoubleVector & result) const {
	result.resize(ncols());
	CPXgetx(_env, _prob, result.data(), 0, ncols() - 1);

}
void CplexSolver::dual(DoubleVector & result) const {
	result.assign(nrows(), 0);
	CPXgetpi(_env, _prob, result.data(), 0, nrows() - 1);
}
void CplexSolver::rc(DoubleVector & result)const{
	result.resize(ncols());
	CPXgetdj(_env, _prob, result.data(), 0, ncols() - 1);
}
int CplexSolver::ncols() const {
	return CPXgetnumcols(_env, _prob);
}
int CplexSolver::nrows() const {
	return CPXgetnumrows(_env, _prob);
}
void CplexSolver::run() {
	CPXchgobjsen(_env, _prob, _is_minimize ? CPX_MIN : CPX_MAX);
	if (_is_mip) {
		CPXmipopt(_env, _prob);
	} else
		CPXlpopt(_env, _prob);
}
char CplexSolver::binary() const {
	return CPX_BINARY;
}
char CplexSolver::continuous() const {
	return CPX_CONTINUOUS;
}

char CplexSolver::leq() const {
	return 'L';
}
char CplexSolver::eq() const {
	return 'E';
}
char CplexSolver::geq() const {
	return 'G';
}

double CplexSolver::infinity() const {
	return CPX_INFBOUND;
}

void CplexSolver::setLog() {
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
	CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY, 2);
}
void CplexSolver::setNoLog() {
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
}
void CplexSolver::setNbThreads(int n) {
	CPXsetintparam(_env, CPX_PARAM_THREADS, n);
}
void CplexSolver::setDual() {
	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_DUAL);
}
#endif

