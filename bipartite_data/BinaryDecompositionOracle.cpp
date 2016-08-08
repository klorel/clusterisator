#include "BinaryDecompositionOracle.h"


#include "../clustering/Timer.h"
#include "../columnsgeneration/Node.h"
#include "BipartiteGraph.h"

BinaryDecompositionOracle::BinaryDecompositionOracle(
		BipartiteGraph const * input) :
		CpxOracle(input), _biPartiteGraph(input) {
	initCpx();
}
BinaryDecompositionOracle::~BinaryDecompositionOracle() {
}
void BinaryDecompositionOracle::initOracle() {
	int err(0);
	throw std::invalid_argument("");
//	if (_prob != NULL) {
//		CPXfreeprob(_env, &_prob);
//		_prob = NULL;
//	}
//	_prob = CPXcreateprob(_env, &err, "Oracle");
//	// 0..R-1 : Yr
//	// R..R+B-1 : Yb
//	ColumnBuffer columnBuffer;
//	_index.resize(_biPartiteGraph->nV());
//	for (size_t v(0); v < _biPartiteGraph->nV(); ++v) {
//		_index[v] = columnBuffer.size();
//		columnBuffer.add(0, CPX_BINARY, 0, 1,
//				v < _biPartiteGraph->nR() ?
//						GetStr("YR_", v) :
//						GetStr("YB_", v - _biPartiteGraph->nR()));
//	}
//
//	//
//	_s = RectMatrix(_biPartiteGraph->nR(), _biPartiteGraph->nB());
//	for (size_t r(0); r < _biPartiteGraph->nR(); ++r) {
//		for (size_t b(0); b < _biPartiteGraph->nB(); ++b) {
//			if (_biPartiteGraph->a(r, b) != 0) {
//				_s.get(r, b) = columnBuffer.size();
////				columnBuffer.add(_biPartiteGraph->a(r, b) * _biPartiteGraph->inv_m(),
////						CPX_CONTINUOUS, 0, CPX_INFBOUND,
////						GetStr("S_", r, "_", b));
//				columnBuffer.add(
//						_biPartiteGraph->a(r, b) * _biPartiteGraph->inv_m(),
//						CPX_CONTINUOUS, -CPX_INFBOUND, CPX_INFBOUND,
//						GetStr("S_", r, "_", b));
//			}
//		}
//	}
//	size_t const tR(
//			(size_t) std::ceil(
//					(std::log(_biPartiteGraph->sum_kR() + 1) / log(2) - 1)));
//	size_t const tB(
//			(size_t) std::ceil(
//					(std::log(_biPartiteGraph->sum_kB() + 1) / log(2) - 1)));
//	IntVector a(tR + 1);
//	for (size_t h(0); h < tR + 1; ++h) {
//		// ah binary
//		a[h] = columnBuffer.size();
//		columnBuffer.add(0, CPX_BINARY, 0, 1, GetStr("a_", h));
//	}
//	IntVector b(tR + 1);
//	for (size_t l(0); l < tB + 1; ++l) {
//		// bl binary
//		b[l] = columnBuffer.size();
//		columnBuffer.add(0, CPX_BINARY, 0, 1, GetStr("b_", l));
//	}
//	_ab = RectMatrix(tR + 1, tB + 1);
//	for (size_t h(0); h < tR + 1; ++h) {
//		for (size_t l(0); l < tB + 1; ++l) {
//			// ahl >= 0
//			_ab.get(h, l) = columnBuffer.size();
//			columnBuffer.add(
//					-std::pow(2, l + h) * _biPartiteGraph->inv_m()
//							* _biPartiteGraph->inv_m(),
//					CPX_CONTINUOUS, 0, CPX_INFBOUND, GetStr("a_", h, "_", l));
//		}
//	}
//	size_t const R1(columnBuffer.size());
//	columnBuffer.add(0, CPX_CONTINUOUS, -CPX_INFBOUND, +CPX_INFBOUND, "R1");
//
//	size_t const B1(columnBuffer.size());
//	columnBuffer.add(0, CPX_CONTINUOUS, -CPX_INFBOUND, +CPX_INFBOUND, "B1");
//	columnBuffer.add(_env, _prob);
//	// binary declaration
//
//	// constraints
//	std::vector<double> rhs;
//	std::vector<char> rtype;
//	std::vector<int> rmatbeg;
//	std::vector<int> rmatind;
//	std::vector<double> rmatval;
//	std::vector<std::string> rname;
//	_rowBuffer.clear();
//	// R1 = sum kr Yr
//	_rowBuffer.add(0, 'E', "R1_k");
//	for (size_t r(0); r < _biPartiteGraph->nR(); ++r) {
//		_rowBuffer.add(r, _biPartiteGraph->k(r));
//	}
//	_rowBuffer.add(R1, -1);
//	// R1 = sum 2^h ah
//	_rowBuffer.add(0, 'E', "R1_POW");
//	for (size_t h(0); h < tR + 1; ++h) {
//		_rowBuffer.add(a[h], std::pow(2, h));
//	}
//	_rowBuffer.add(R1, -1);
//	// B1 = sum kb Yb
//	_rowBuffer.add(0, 'E', "B1_k");
//	for (size_t b(0); b < _biPartiteGraph->nB(); ++b) {
//		_rowBuffer.add(_biPartiteGraph->nR() + b,
//				_biPartiteGraph->k(_biPartiteGraph->nR() + b));
//	}
//	_rowBuffer.add(B1, -1);
//	// B1 = sum 2^l al
//	_rowBuffer.add(0, 'E', "B1_POW");
//	for (size_t l(0); l < tB + 1; ++l) {
//		_rowBuffer.add(b[l], std::pow(2, l));
//	}
//	_rowBuffer.add(B1, -1);
//	for (size_t r(0); r < _biPartiteGraph->nR(); ++r) {
//		for (size_t b(0); b < _biPartiteGraph->nB(); ++b) {
//			if (_biPartiteGraph->a(r, b) != 0) {
//				// Srb <= Yr (r,b) in E
//				_rowBuffer.add(0, 'L', GetStr("S_LEQ_R_R", r, "_B", b));
//				_rowBuffer.add(r, -1);
//				_rowBuffer.add((int) _s.get(r, b), +1);
//				// Srb <= Yb (r,b) in E
//				_rowBuffer.add(0, 'L', GetStr("S_LEQ_B_R", r, "_B", b));
//				_rowBuffer.add(_biPartiteGraph->nR() + b, -1);
//				_rowBuffer.add((int) _s.get(r, b), +1);
////				// Srb >= Yr+ Yb -1  (r,b) in E
////				_rowBuffer.add(-1, 'G', GetStr("SYRB_", r, "_", b));
////				_rowBuffer.add(r, -1);
////				_rowBuffer.add(_biPartiteGraph->nR() + b, -1);
////				_rowBuffer.add((int) _s.get(r, b), +1);
//			}
//		}
//	}
//	// abhl >= ah+bl-1
//	for (size_t h(0); h < tR + 1; ++h) {
//		for (size_t l(0); l < tB + 1; ++l) {
//			_rowBuffer.add(-1, 'G', GetStr("FORTET_", h, "_", l));
//			_rowBuffer.add((int) _ab.get(h, l), 1);
//			_rowBuffer.add(a[h], -1);
//			_rowBuffer.add(b[l], -1);
//		}
//	}
////	// abhl <= ah
////	for (size_t h(0); h < tR + 1; ++h) {
////		for (size_t l(0); l < tB + 1; ++l) {
////			// abhl <= ah
////			_rowBuffer.add(0, 'L', GetStr("abh_", h, "_", l));
////			_rowBuffer.add((int) _ab.get(h, l), 1);
////			_rowBuffer.add(a[h], -1);
////			// abhl <= bl
////			_rowBuffer.add(0, 'L', GetStr("abl_", h, "_", l));
////			_rowBuffer.add((int) _ab.get(h, l), 1);
////			_rowBuffer.add(b[l], -1);
////		}
////	}
//	_rowBuffer.add(_env, _prob);
//	CPXchgobjsen(_env, _prob, -1);
//	//writeOracle();
//	//CPXmipopt(_env, _oracle);
//	//double toto;
//	//CPXgetobjval(_env, _oracle, &toto);
//	//std::cout << "toto : "<<toto<<std::endl;
}

void BinaryDecompositionOracle::setUpOracle() {
	throw std::invalid_argument("");
//	_columns.clear();
//	CPXchgobj(_env, _prob, (int) _index.size(), _index.data(), _dual->data());
//
////	for (auto const & i : _index) {
////		std::cout << std::setw(6) << i;
////		std::cout << std::setw(25) << _dual->at(i);
////		std::cout << std::endl;
////	}
//	//	write();
//	if (CPXgetnummipstarts(_env, _prob) > 1)
//		CPXdelmipstarts(_env, _prob, 0, CPXgetnummipstarts(_env, _prob) - 1);
}
void BinaryDecompositionOracle::checkSolutions() {
	throw std::invalid_argument("");
//	CpxOracle::checkSolutions();
//	DoubleVector x(CPXgetnumcols(_env, _prob));
//	size_t const n(1 + 0 * CPXgetsolnpoolnumsolns(_env, _prob));
//	//			std::cout << std::setw(4) << n << std::endl;
//	Double obj;
//	for (size_t i(0); i < n; ++i) {
//		CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
//		CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0,
//				(int) (x.size() - 1));
//		Column column(_biPartiteGraph);
//
//		for (size_t v(0); v < _biPartiteGraph->nV(); ++v) {
//			if (x[v] > 0.5) {
//				column.insert(v);
//			}
//		}
//		for (size_t r(0); r < _biPartiteGraph->nR(); ++r) {
//			for (size_t b(0); b < _biPartiteGraph->nB(); ++b) {
//				if (_biPartiteGraph->a(r, b) != 0) {
//					if (std::fabs(
//							x[r] * x[_biPartiteGraph->nR() + b]
//									- x[_s.get(r, b)]) > 0.5) {
//						std::cout << std::setw(8) << "ERROR S";
//						std::cout << std::setw(8) << _biPartiteGraph->a(r, b);
//						std::cout << std::setw(4) << "Y_R" << r;
//						std::cout << std::setw(4) << x[r];
//						std::cout << std::setw(4) << "Y_B" << b;
//						std::cout << std::setw(4)
//								<< x[_biPartiteGraph->nR() + b];
//						std::cout << std::setw(4) << x[_s.get(r, b)];
//						std::cout << std::endl;
//					}
//				}
//			}
//		}
//	}
}

