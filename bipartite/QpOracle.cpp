#include "QpOracle.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"

#include <cplex.h>

QpOracle::QpOracle(BipartiteGraph const * input, DoubleVector const * dual,
		DecisionList const * decisions) :
		IOracle(input, dual, decisions), _env(NULL), _oracle(NULL), _index() {
	_diagRegularisation = 1;
	initCpx();
}
void QpOracle::initCpx() {
	int err;
	_env = CPXopenCPLEX(&err);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
//	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
//	CPXsetintparam(_env, CPX_PARAM_CUTPASS, -1);
//	CPXsetintparam(_env, CPX_PARAM_VARSEL, 4);
	CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY, 2);
	initOracle();

}
QpOracle::~QpOracle() {
	freeLp();
}
void QpOracle::freeLp() {
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
	if (_oracle != NULL) {
		CPXfreeprob(_env, &_oracle);
		_oracle = NULL;
	}
}
void QpOracle::initOracle() {
	int err(0);
	if (_oracle != NULL) {
		CPXfreeprob(_env, &_oracle);
		_oracle = NULL;
	}
	_oracle = CPXcreateprob(_env, &err, "Oracle");
	// 0..R-1 : Yr
	// R..R+B-1 : Yb
	ColumnBuffer columnBuffer;
	_index.resize(_input->nV());
	for (size_t v(0); v < _input->nV(); ++v) {
		_index[v] = columnBuffer.size();
		columnBuffer.add(0, CPX_BINARY, 0, 1,
				v < _input->nR() ?
						GetStr("YR_", v) : GetStr("YB_", v - _input->nR()));
	}

	// Srb=Yr.Yb
	std::map<size_t, std::map<size_t, Double> > q;
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			if (_input->w(r, b) != 0) {
				q[r][_input->nR() + b] = _input->w(r, b);
				q[_input->nR() + b][r] = _input->w(r, b);
			}
		}
	}
	for (size_t v(0); v < _input->nV(); ++v)
		q[v][v] = -2 * _diagRegularisation;
	columnBuffer.add(_env, _oracle);
	// quadratic declaration

	std::vector<int> qmatbeg(_input->nV(), 0);
	std::vector<int> qmatcnt(_input->nV(), 0);
	std::vector<int> qmatind;
	std::vector<double> qmatval;

	for (auto const & x : q) {
		qmatbeg[x.first] = qmatval.size();
		qmatcnt[x.first] = x.second.size();
		for (auto const & y : x.second) {
			qmatval.push_back(y.second);
			qmatind.push_back(y.first);
		}
	}
	CPXchgobjsen(_env, _oracle, -1);
	CPXcopyquad(_env, _oracle, qmatbeg.data(), qmatcnt.data(), qmatind.data(),
			qmatval.data());
//	generate();
//	exit(-1);
	//writeOracle();
	//CPXmipopt(_env, _oracle);
	//double toto;
	//CPXgetobjval(_env, _oracle, &toto);
	//std::cout << "toto : "<<toto<<std::endl;
}

// on initialise avec les singletons
void QpOracle::write(std::string const & fileName) const {
	CPXwriteprob(_env, _oracle, fileName.c_str(), "LP");
}

void QpOracle::setUpOracle() {
	_columns.clear();
//	CPXchgobj(_env, _oracle, (int) _index.size(), _index.data(), _dual->data());
	DoubleVector copyDual(*_dual);

	for (size_t v(0); v < _input->nV(); ++v)
		copyDual[v] += _diagRegularisation;
	CPXchgobj(_env, _oracle, (int) _index.size(), _index.data(),
			copyDual.data());
	//	for (auto const & i : _index) {
	//		std::cout << std::setw(6) << i;
	//		std::cout << std::setw(25) << _dual[i];
	//		std::cout << std::endl;
	//	}
	//	writeOracle();
	if (CPXgetnummipstarts(_env, _oracle) > 1)
		CPXdelmipstarts(_env, _oracle, 0,
				CPXgetnummipstarts(_env, _oracle) - 1);
}
bool QpOracle::generate() {
	setUpOracle();
//	write();
	CPXmipopt(_env, _oracle);
//	checkSolution();
	//	CPXpopulate(_env, _oracle);
	bool result(false);
	_bestReducedCost = ZERO_REDUCED_COST;
	if (CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL
			|| CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL_TOL
			|| CPXgetstat(_env, _oracle) == CPXMIP_POPULATESOL_LIM
			|| CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL_POPULATED
			|| CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL_POPULATED_TOL) {
		CPXgetobjval(_env, _oracle, &_bestReducedCost);
		//_rd-=_cstDual;
//		std::cout << "_bestReducedCost : " << _bestReducedCost << std::endl;
		result = (_bestReducedCost > ZERO_REDUCED_COST);
		if (result) {
			DoubleVector x(CPXgetnumcols(_env, _oracle));
			size_t const n(CPXgetsolnpoolnumsolns(_env, _oracle));
			//			std::cout << std::setw(4) << n << std::endl;
			Double obj;
			for (size_t i(0); i < n; ++i) {
				CPXgetsolnpoolobjval(_env, _oracle, (int) i, &obj);
				if (obj > ZERO_REDUCED_COST) {
					CPXgetsolnpoolx(_env, _oracle, (int) i, x.data(), 0,
							(int) (x.size() - 1));
					Column column(_input);
					for (size_t r(0); r < _input->nR(); ++r) {
						if (x[r] > 0.5) {
							column.r().insert(r);
						}
					}
					for (size_t b(0); b < _input->nB(); ++b) {
						if (x[_input->nR() + b] > 0.5) {
							column.b().insert(b);
						}
					}
					column.cost() = column.computeCost();
					column.reducedCost() = obj;
					column.check(*_dual);
					for (Decision const & decision : *_decisions) {
						if (column.violation(decision) > 0) {
							decision.print(
									std::cout
											<< "violation in MipGenerator::generate() ");
							std::cout << std::endl;
							column.print();
						}
					}
//					write();
//					assert(column.violation(*_decisions) == 0);
					_columns.insert(column);

				}
			}
		}
	} else
		std::cout << "CPXgetstat : " << CPXgetstat(_env, _oracle) << std::endl;
	return result;
}

void QpOracle::applyBranchingRule() {
	if (_rowBuffer.size() != CPXgetnumrows(_env, _oracle))
		CPXdelrows(_env, _oracle, _rowBuffer.size(),
				CPXgetnumrows(_env, _oracle) - 1);
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
		_decisionBuffer.add(_env, _oracle);
	}
}

void QpOracle::checkSolution() const {
	Double obj;
	DoubleVector x(CPXgetnumcols(_env, _oracle));
	size_t const n(CPXgetsolnpoolnumsolns(_env, _oracle));
	for (size_t i(0); i < n; ++i) {
		CPXgetsolnpoolobjval(_env, _oracle, (int) i, &obj);
		std::cout << "SOLUTION" << std::setw(2) << i << std::setw(10) << obj
				<< std::endl;
		CPXgetsolnpoolx(_env, _oracle, (int) i, x.data(), 0,
				(int) (x.size() - 1));
		Column column(_input);
		for (size_t r(0); r < _input->nR(); ++r) {
			if (x[r] > 0.5) {
				column.r().insert(r);
			}
		}
		for (size_t b(0); b < _input->nB(); ++b) {
			if (x[_input->nR() + b] > 0.5) {
				column.b().insert(b);
			}
		}
		column.cost() = column.computeCost();
		column.reducedCost() = obj;
		column.check(*_dual);
		for (Decision const & decision : *_decisions) {
			if (column.violation(decision) > 0) {
				decision.print(
						std::cout << "violation in MipGenerator::generate() ");
				std::cout << std::endl;
				column.print();
			}
		}
	}
}
