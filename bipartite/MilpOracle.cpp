#include "MilpOracle.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"

#include <cplex.h>

MilpOracle::MilpOracle(BipartiteGraph const * input, DoubleVector const * dual,
		DecisionList const * decisions) :
		CpxOracle(input, dual, decisions) {
	initCpx();
}
void MilpOracle::initCpx() {
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
MilpOracle::~MilpOracle() {
}
void MilpOracle::initOracle() {
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
		columnBuffer.add(0, CPX_BINARY, 0, 1,
				v < _input->nR() ?
						GetStr("YR_", v) : GetStr("YB_", v - _input->nR()));
	}

// Srb=Yr.Yb
	_s = RectMatrix(_input->nR(), _input->nB());
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			if (_input->w(r, b) != 0) {
				_s.get(r, b) = columnBuffer.size();
				columnBuffer.add(_input->w(r, b), CPX_CONTINUOUS,
						_input->w(r, b) < 0 ? 0 : -CPX_INFBOUND, CPX_INFBOUND,
						GetStr("S_", r, "_", b));
			}
		}
	}
	columnBuffer.add(_env, _prob);
// binary declaration

// constraints
	std::vector<double> rhs;
	std::vector<char> rtype;
	std::vector<int> rmatbeg;
	std::vector<int> rmatind;
	std::vector<double> rmatval;
	std::vector<std::string> rname;
	_rowBuffer.clear();
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			if (_input->w(r, b) > 0) {
				// Srb <= Yr (r,b) in RxB
				_rowBuffer.add(0, 'L', GetStr("S_LEQ_R_R", r, "_B", b));
				_rowBuffer.add(r, -1);
				_rowBuffer.add((int) _s.get(r, b), +1);
				// Srb <= Yb (r,b) in RxB
				_rowBuffer.add(0, 'L', GetStr("S_LEQ_B_R", r, "_B", b));
				_rowBuffer.add(_input->nR() + b, -1);
				_rowBuffer.add((int) _s.get(r, b), +1);
			} else if (_input->w(r, b) < 0) {
				// Srb >= 0 already in
				// Srb >= Yr+Yb-1
				_rowBuffer.add(-1, 'G', GetStr("S_GEQ_RB_R_", r, "_YB", b));
				_rowBuffer.add(r, -1);
				_rowBuffer.add(_input->nR() + b, -1);
				_rowBuffer.add((int) _s.get(r, b), +1);
			}
		}
	}
	_rowBuffer.add(_env, _prob);
	CPXchgobjsen(_env, _prob, -1);
//writeOracle();
//CPXmipopt(_env, _oracle);
//double toto;
//CPXgetobjval(_env, _oracle, &toto);
//std::cout << "toto : "<<toto<<std::endl;
}


void MilpOracle::setUpOracle() {
	_columns.clear();
	CPXchgobj(_env, _prob, (int) _index.size(), _index.data(), _dual->data());

//	for (auto const & i : _index) {
//		std::cout << std::setw(6) << i;
//		std::cout << std::setw(25) << _dual->at(i);
//		std::cout << std::endl;
//	}
//	writeOracle();
	if (CPXgetnummipstarts(_env, _prob) > 1)
		CPXdelmipstarts(_env, _prob, 0,
				CPXgetnummipstarts(_env, _prob) - 1);
}

void MilpOracle::checkMipSolution() const {
	Double obj;
	DoubleVector x(CPXgetnumcols(_env, _prob));
	size_t const n(CPXgetsolnpoolnumsolns(_env, _prob));
	for (size_t i(0); i < n; ++i) {
		CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
		std::cout << "SOLUTION" << std::setw(2) << i << std::setw(10) << obj
				<< std::endl;
		for (size_t r(0); r < _input->nR(); ++r) {
			for (size_t b(0); b < _input->nB(); ++b) {
				double const yR(x[r]);
				double const yB(x[_input->nR() + b]);
				double const sRB(x[_s.get(r, b)]);
//				std::cout << std::setw(4) << yR;
//				std::cout << std::setw(4) << yB;
//				std::cout << std::setw(4) << sRB;
//				std::cout << std::endl;
				assert(std::abs(yR * yB - sRB) < 1e-10);

			}
		}
	}
}
