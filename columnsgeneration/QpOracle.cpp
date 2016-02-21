#include "QpOracle.h"

#include "../clustering/Timer.h"
#include "Column.h"

QpOracle::QpOracle(CliquePartitionProblem const * input,
		DoubleVector const * dual, DecisionList const * decisions) :
		CpxOracle(input, dual, decisions) {
	_diagRegularisation = 1e2 * 0;
	initCpx();
}
QpOracle::~QpOracle() {
}
void QpOracle::initOracle() {
	int err(0);
	if (_prob != NULL) {
		CPXfreeprob(_env, &_prob);
		_prob = NULL;
	}
	_prob = CPXcreateprob(_env, &err, "Oracle");
	// 0..R-1 : Yr
	// R..R+B-1 : Yb
	ColumnBuffer columnBuffer;
	_index.resize(_input->nV());
	for (size_t v(0); v < _input->nV(); ++v) {
		_index[v] = columnBuffer.size();
		columnBuffer.add(0, CPX_BINARY, 0, 1, _input->name(v));
	}

	// Srb=Yr.Yb
	std::map<size_t, std::map<size_t, Double> > q;
	for (auto const & edge : _input->costs()) {
		if (edge._v != 0) {
			q[edge._j][edge._i] = edge._v;
			q[edge._i][edge._j] = edge._v;
		}
	}
//	for (size_t r(0); r < _input->nR(); ++r) {
//		for (size_t b(0); b < _input->nB(); ++b) {
//			if (_input->w(r, b) != 0) {
//				q[r][_input->nR() + b] = _input->w(r, b);
//				q[_input->nR() + b][r] = _input->w(r, b);
//			}
//		}
//	}
	for (size_t v(0); v < _input->nV(); ++v)
		q[v][v] = -2 * _diagRegularisation;
	columnBuffer.add(_env, _prob);
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
	CPXchgobjsen(_env, _prob, -1);
	CPXcopyquad(_env, _prob, qmatbeg.data(), qmatcnt.data(), qmatind.data(),
			qmatval.data());
//	CPXchgprobtype(_env, _oracle, CPXPROB_MIQP);
//	CPXqpopt(_env, _oracle);
//	generate();

//	write();
//	exit(-1);
	//CPXmipopt(_env, _oracle);
	//double toto;
	//CPXgetobjval(_env, _oracle, &toto);
	//std::cout << "toto : "<<toto<<std::endl;
}

void QpOracle::setUpOracle() {
	_columns.clear();
//	CPXchgobj(_env, _oracle, (int) _index.size(), _index.data(), _dual->data());
	DoubleVector copyDual(*_dual);

	for (size_t v(0); v < _input->nV(); ++v)
		copyDual[v] += _diagRegularisation;
	CPXchgobj(_env, _prob, (int) _index.size(), _index.data(), copyDual.data());
	//	for (auto const & i : _index) {
	//		std::cout << std::setw(6) << i;
	//		std::cout << std::setw(25) << _dual[i];
	//		std::cout << std::endl;
	//	}
	//	writeOracle();
	if (CPXgetnummipstarts(_env, _prob) > 1)
		CPXdelmipstarts(_env, _prob, 0, CPXgetnummipstarts(_env, _prob) - 1);
}
bool QpOracle::generate() {
	setUpOracle();
//	write();
	CPXsetintparam(_env, CPX_PARAM_SOLUTIONTARGET,
	CPX_SOLUTIONTARGET_OPTIMALGLOBAL);
	CPXmipopt(_env, _prob);
//	CPXqpopt(_env, _prob);

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
	} else {
		std::cout << "CPXgetstat : " << CPXgetstat(_env, _prob) << std::endl;
		std::cout << "exporting lp" << std::endl;
		write();
		__SEG_FAULT__;
	}
	return result;
}

void QpOracle::checkSolution() const {

}

