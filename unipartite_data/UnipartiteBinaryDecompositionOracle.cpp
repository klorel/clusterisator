#include "UnipartiteBinaryDecompositionOracle.h"
#include "../clustering/Timer.h"
#include "../columnsgeneration/Node.h"

UnipartiteBinaryDecompositionOracle::UnipartiteBinaryDecompositionOracle(UnipartieInstance const * input) :
		CpxOracle(input), _uniPartiteGraph(input) {
	_tD = 0;
	_c = 0;
	initCpx();
	//_solver->setLog();
}

UnipartiteBinaryDecompositionOracle::~UnipartiteBinaryDecompositionOracle() {
}

void UnipartiteBinaryDecompositionOracle::fill(RowBuffer & rowBuffer, ColumnBuffer & columnBuffer) {
	char const binary(_solver->binary());
	char const continuous(_solver->continuous());
	double const infinity(_solver->infinity());

	int const n(_input->nV());
	int const nn(n * (n - 1) / 2);
	_index.resize(n);
	for (int v(0); v < n; ++v) {
		_index[v] = columnBuffer.size();
		columnBuffer.add(0, binary, 0, 1, GetStr("Y_", v));
	}

	// s = Yi Yj
	_s.resize(nn);
	for (auto const & edge : _uniPartiteGraph->edges()) {
		int const i(edge._i);
		int const j(edge._j);
		int const ij(ijtok(n, i, j));
		_s[ij] = columnBuffer.size();
		double const cost(edge._v * _uniPartiteGraph->inv_m());
		columnBuffer.add(cost, continuous, -infinity, infinity, GetStr("S_V", i, "_V", j));
	}

	_tD = (int) std::ceil((std::log(_uniPartiteGraph->sum_k() + 1) / log(2) - 1));
	double const tempCoeff(_uniPartiteGraph->inv_m() * _uniPartiteGraph->inv_m() * 0.5 * 0.5);

	_a.resize(_tD + 1);
	for (int h(0); h < _tD + 1; ++h) {
		// ah binary
		_a[h] = columnBuffer.size();
		columnBuffer.add(-std::pow(2, 2 * h) * tempCoeff, binary, 0, 1, GetStr("a_", h));
	}
	//	_aa = RectMatrix(_tD + 1, _tD + 1, 0);
	_aa.resize((_tD + 1) * _tD * 0.5);
	for (int h(0); h < _tD + 1; ++h) {
		for (int l(h + 1); l < _tD + 1; ++l) {
			// ahl >= 0
			_aa[ijtok(_tD + 1, h, l)] = columnBuffer.size();
			double const cost(-std::pow(2, h + l + 1) * tempCoeff);
			columnBuffer.add(cost, continuous, 0, infinity, GetStr("a_H", h, "_L", l));
		}
	}

	int const D(columnBuffer.size());
	columnBuffer.add(0, continuous, -infinity, +infinity, "D");

	_c = columnBuffer.size();
	columnBuffer.add(tempCoeff, continuous, -infinity, +infinity, "C");
	// constraints
	rowBuffer.clear();
	// C = sum kr² Yr
	_rowBuffer.add(0, 'E', "C_k");
	for (int v(0); v < n; ++v) {
		_rowBuffer.add(v, _uniPartiteGraph->k(v) * _uniPartiteGraph->k(v));
	}
	_rowBuffer.add(_c, -1);

	// D = sum kr Yr
	rowBuffer.add(0, 'E', "D_k");
	for (int v(0); v < n; ++v) {
		rowBuffer.add(v, _uniPartiteGraph->k(v));
	}
	rowBuffer.add(D, -1);
	// D = sum 2^h ah
	rowBuffer.add(0, 'E', "D_POW");
	for (int h(0); h < _tD + 1; ++h) {
		rowBuffer.add(_a[h], std::pow(2, h));
	}
	rowBuffer.add(D, -1);
	for (auto const & edge : _uniPartiteGraph->edges()) {
		int const i(edge._i);
		int const j(edge._j);
		int const ij(ijtok(n, i, j));
		// Srb <= Yr (r,b) in E
		rowBuffer.add(0, 'L', GetStr("S_LEQ_R_V", i, "_V", j));
		rowBuffer.add(i, -1);
		rowBuffer.add(_s[ij], +1);
		// Srb <= Yb (r,b) in E
		rowBuffer.add(0, 'L', GetStr("S_LEQ_B_V", i, "_V", j));
		rowBuffer.add(j, -1);
		rowBuffer.add(_s[ij], +1);
	}
	// abhl >= ah+bl-1
	for (int h(0); h < _tD + 1; ++h) {
		for (int l(h + 1); l < _tD + 1; ++l) {
			rowBuffer.add(-1, 'G', GetStr("FORTET_H", h, "_L", l));
			if (h == l) {
				rowBuffer.add(_a[h], -2);
			} else {
				rowBuffer.add(_a[h], -1);
				rowBuffer.add(_a[l], -1);
			}
			int const hl(ijtok(_tD + 1, h, l));
			rowBuffer.add(_aa[hl], 1);
		}
	}
}

void UnipartiteBinaryDecompositionOracle::initOracle() {
	_solver->initLp("UnipartiteBinaryDecompositionOracle");
	ColumnBuffer columnBuffer(_solver->continuous());
	fill(_rowBuffer, columnBuffer);
	_solver->add(columnBuffer);
	_solver->add(_rowBuffer);
	_solver->maximize();
//	checkSolutions();

//	CPXwriteprob(_env,_prob,"bMILP.lp","LP");
//	CPXmipopt(_env, _prob);
//	double toto;
//	CPXgetobjval(_env, _prob, &toto);
//	std::cout << "toto : " << toto << std::endl;
//	std::exit(0);
}

void UnipartiteBinaryDecompositionOracle::setUpOracle() {
	_columns.clear();
	_solver->chgObj(_index, *_dual);
//	for (auto const & i : _index) {
//		std::cout << std::setw(6) << i;
//		std::cout << std::setw(25) << _dual->at(i);
//		std::cout << std::endl;
//	}
	//	write();
	_solver->delMipStarts();

}

void UnipartiteBinaryDecompositionOracle::checkSolutions() const {
	throw std::invalid_argument("");
//	CpxOracle::checkSolutions();
//	DoubleVector x(CPXgetnumcols(_env, _prob));
//	int const n(1 + 0 * CPXgetsolnpoolnumsolns(_env, _prob));
//	//			std::cout << std::setw(4) << n << std::endl;
//	Double obj;
//
//	CPXwriteprob(_env, _prob, "bMILP.lp", "LP");
//
//	for (int i(0); i < n; ++i) {
//		CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
//		CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0,
//				(int) (x.size() - 1));
//		Column column(_uniPartiteGraph);
//		double verifD(0);
//		for (int v(0); v < _uniPartiteGraph->nV(); ++v) {
//			if (x[v] > 0.5) {
//				column.insert(v);
//				verifD += _uniPartiteGraph->k(v);
//			}
//		}
//		for (auto const & edge : _uniPartiteGraph->edges()) {
//			int const i(edge._i);
//			int const j(edge._j);
//			int const ij(ijtok(_uniPartiteGraph->nV(), i, j));
//			if (std::fabs(x[i] * x[j] - x[_s[ij]]) > 0.5) {
//				std::cout << std::setw(8) << "ERROR S";
//				std::cout << std::setw(4) << "Y_V" << i;
//				std::cout << std::setw(4) << x[i];
//				std::cout << std::setw(4) << "Y_V" << i;
//				std::cout << std::setw(4) << x[j];
//				std::cout << std::setw(4) << x[_s[ij]];
//				std::cout << std::endl;
//				std::exit(0);
//			}
//
//		}
//		// abhl >= ah+bl-1
//		for (int h(0); h < _tD + 1; ++h) {
//			for (int l(h + 1); l < _tD + 1; ++l) {
//				int const hl(ijtok(_tD + 1, h, l));
//				if (std::fabs(x[_a[h]] * x[_a[l]] - x[hl]) > 0.5) {
//					std::cout << std::setw(8) << "ERROR AA";
//					std::cout << std::setw(8) << "a_" << h;
//					std::cout << std::setw(4) << x[_a[h]];
//					std::cout << std::setw(8) << "a_" << l;
//					std::cout << std::setw(4) << x[_a[l]];
//					std::cout << std::setw(8) << GetStr("a_H", h, "_L", l);
//					std::cout << std::setw(4) << x[hl];
//					std::cout << std::endl;
//					std::exit(0);
//				}
//			}
//		}
//
//		double local(0);
//		double verifMod(0);
//		for (auto const & edge : _uniPartiteGraph->edges()) {
//			int const i(edge._i);
//			int const j(edge._j);
//			int const ij(ijtok(_uniPartiteGraph->nV(), i, j));
//			if (x[_s[ij]] > 0.5) {
//				++local;
//				verifMod += _uniPartiteGraph->inv_m();
//			}
//		}
//		for (int u(0); u < _uniPartiteGraph->nV(); ++u) {
//			for (int v(u + 1); v < _uniPartiteGraph->nV(); ++v) {
//				if (x[u] > 0.5 && x[v] > 0.5)
//					verifMod -= (_uniPartiteGraph->k(u) * _uniPartiteGraph->k(v)
//							* _uniPartiteGraph->inv_m()
//							* _uniPartiteGraph->inv_m() * .5);
//			}
//		}
//		double binD(0);
//		for (int h(0); h < _tD + 1; ++h) {
//			binD += x[_a[h]] * std::pow(2, h);
////			if (x[_a[h]] > 0.5) {
////				std::cout << std::setw(4) << h;
////				std::cout << std::setw(4) << std::pow(2, h);
////				std::cout << std::endl;
////			}
//		}
//		double binD2(0);
//		for (int h(0); h < _tD + 1; ++h) {
//			for (int l(h + 1); l < _tD + 1; ++l) {
//				int const hl(ijtok(_tD + 1, h, l));
//				binD2 += x[hl] * std::pow(2, h + l);
////				if (x[hl] > 0.5) {
////					std::cout << std::setw(4) << h;
////					std::cout << std::setw(4) << l;
////					std::cout << std::setw(4) << std::pow(2, h + l);
////					std::cout << std::endl;
////				}
//			}
//		}
//		if (std::fabs(binD - verifD) > 1e-6) {
//			std::cout << "WRONG binD != verifD" << std::endl;
//
//			MY_PRINT(verifD);
//			MY_PRINT(binD);
//			MY_PRINT(binD * binD);
//			MY_PRINT(binD2);
//			std::exit(0);
//		}
//
//		if (std::fabs(binD2 - verifD * verifD) > 1e-6) {
//			std::cout << "WRONG binD2 != verifD*verifD" << std::endl;
//
//			MY_PRINT(verifD);
//			MY_PRINT(binD);
//			MY_PRINT(binD * binD);
//			MY_PRINT(binD2);
//			std::exit(0);
//		}
//		double temp(0);
//		for (int v(0); v < _uniPartiteGraph->nV(); ++v) {
//			if (x[v] > 0.5) {
//				temp += _uniPartiteGraph->k(v) * _uniPartiteGraph->k(v);
//			}
//		}
//		double computedObj(
//				local * _uniPartiteGraph->inv_m()
//						- 0.5 * _uniPartiteGraph->inv_m()
//								* _uniPartiteGraph->inv_m() * 0.5
//								* (binD2 - temp));
//		MY_PRINT(computedObj);
//		MY_PRINT(verifMod);
//		MY_PRINT(verifD);
//		MY_PRINT(binD);
//		MY_PRINT(binD * binD);
//		MY_PRINT(binD2);
//
//	}
}

