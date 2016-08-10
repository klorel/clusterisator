/*
 * CplexSolver.cpp
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#include "CplexSolver.h"
#include "LpBuffer.h"
CplexSolver::CplexSolver() {
	// TODO Auto-generated constructor stub

}

CplexSolver::~CplexSolver() {
	// TODO Auto-generated destructor stub
}
#ifndef __LAZY_CPLEX__
#include <cplex.h>
void CplexSolver::add(RowBuffer  & buffer) {
	buffer.add_last_begin();	
	if (buffer.name().empty()) {
		CPXaddrows(_env, _prob, 0, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.type(), buffer.begin(), buffer.index(), buffer.value(), NULL, NULL);
	} else {
		//		MY_PRINT(_name.size());
		//		MY_PRINT(size());
		assert((int )buffer.name().size() == buffer.size() && "you should provide a name for each element");

		std::vector<char*> cpxName(buffer.name().size());
		for (size_t i(0); i < buffer.name().size(); ++i) {
			cpxName[i] = const_cast<char*>(buffer.name()[i].c_str());
		}
		CPXaddrows(_env, _prob, 0, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.type(), buffer.begin(), buffer.index(), buffer.value(), NULL, cpxName.data());
	}
	buffer.rem_last_begin();
}
void CplexSolver::add(ColumnBuffer & buffer) {
	buffer.add_last_begin();
	if (buffer.name().empty()) {
		CPXaddcols(_env, _prob, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.begin(),
			buffer.index(), buffer.value(), buffer.lower(), buffer.upper(), NULL);
	} else {
		assert((int )buffer.name().size() == buffer.size()&& "you should provide a name for each element");

		std::vector<char*> cpxName(buffer.name().size());
		for (size_t i(0); i < buffer.name().size(); ++i) {
			cpxName[i] = const_cast<char*>(buffer.name()[i].c_str());
		}
		CPXaddcols(_env, _prob, buffer.size(), buffer.nz(), buffer.rhsObj(), buffer.begin(),
			buffer.index(), buffer.value(), buffer.lower(), buffer.upper(), cpxName.data());
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
	//CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
	//	CPXsetintparam(_env, CPX_PARAM_PREPASS, 0);
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

void CplexSolver::chgObj(IntVector const & indexe,
		DoubleVector const & values) {

}

void CplexSolver::delMipStarts() {
}

bool CplexSolver::isOptimal() const {
	return CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL
			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_TOL
			|| CPXgetstat(_env, _prob) == CPXMIP_POPULATESOL_LIM
			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED
			|| CPXgetstat(_env, _prob) == CPXMIP_OPTIMAL_POPULATED_TOL;
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

void CplexSolver::setUpOracle() {

}
void CplexSolver::initOracle() {

}
double CplexSolver::objValue()const {
	double result(0);
	return CPXgetobjval(_env, _prob, &result);
	return result;
}


void CplexSolver::run() {
	CPXchgobjsen(_env, _prob, _is_minimize ? CPX_MIN : CPX_MAX);
	if (_is_mip)
		CPXmipopt(_env, _prob);
	else
		CPXoptimize(_env, _prob);
}
char CplexSolver::binary()const { 
	return CPX_BINARY; 
}
char CplexSolver::continuous()const { 
	return CPX_CONTINUOUS; 
}

char CplexSolver::leq()const { 
	return 'L';
}
char CplexSolver::eq()const { 
	return 'E';
}
char CplexSolver::geq()const { 
	return 'G'; 
}

#endif