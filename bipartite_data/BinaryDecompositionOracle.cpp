#include "BinaryDecompositionOracle.h"

#include "../clustering/Timer.h"
#include "../columnsgeneration/Node.h"
#include "BipartiteGraph.h"

BinaryDecompositionOracle::BinaryDecompositionOracle(
    BipartiteGraph const * input)
    : CpxOracle(input),
      _biPartiteGraph(input) {
  initCpx();
}
BinaryDecompositionOracle::~BinaryDecompositionOracle() {
}

void BinaryDecompositionOracle::fill(RowBuffer & rowBuffer, ColumnBuffer & columnBuffer) {
	columnBuffer.clear();
	char const binary(_solver->binary());
	char const continuous(_solver->continuous());
	double const infinity(_solver->infinity());

	// 0..R-1 : Yr
	// R..R+B-1 : Yb
	_index.resize(_biPartiteGraph->nV());
	for (int v(0); v < _biPartiteGraph->nV(); ++v) {
		_index[v] = columnBuffer.size();
		columnBuffer.add(
			0,
			binary,
			0,
			1,
			v < _biPartiteGraph->nR() ?
			GetStr("YR_", v) : GetStr("YB_", v - _biPartiteGraph->nR()));
	}

	//
	_s = RectMatrix(_biPartiteGraph->nR(), _biPartiteGraph->nB());
	for (int r(0); r < _biPartiteGraph->nR(); ++r) {
		for (int b(0); b < _biPartiteGraph->nB(); ++b) {
			if (_biPartiteGraph->a(r, b) != 0) {
				_s.get(r, b) = columnBuffer.size();
				//				columnBuffer.add(_biPartiteGraph->a(r, b) * _biPartiteGraph->inv_m(),
				//						continuous, 0, infinity,
				//						GetStr("S_", r, "_", b));
				columnBuffer.add(_biPartiteGraph->a(r, b) * _biPartiteGraph->inv_m(),
					continuous, -infinity, infinity,
					GetStr("S_", r, "_", b));
			}
		}
	}
	int const tR(
		(int)std::ceil(
		(std::log(_biPartiteGraph->sum_kR() + 1) / log(2) - 1)));
	int const tB(
		(int)std::ceil(
		(std::log(_biPartiteGraph->sum_kB() + 1) / log(2) - 1)));
	IntVector a(tR + 1);
	for (int h(0); h < tR + 1; ++h) {
		// ah binary
		a[h] = columnBuffer.size();
		columnBuffer.add(0, binary, 0, 1, GetStr("a_", h));
	}
	IntVector b(tR + 1);
	for (int l(0); l < tB + 1; ++l) {
		// bl binary
		b[l] = columnBuffer.size();
		columnBuffer.add(0, binary, 0, 1, GetStr("b_", l));
	}
	_ab = RectMatrix(tR + 1, tB + 1);
	for (int h(0); h < tR + 1; ++h) {
		for (int l(0); l < tB + 1; ++l) {
			// ahl >= 0
			_ab.get(h, l) = columnBuffer.size();
			columnBuffer.add(
				-std::pow(2, l + h) * _biPartiteGraph->inv_m()
				* _biPartiteGraph->inv_m(),
				continuous, 0, infinity, GetStr("a_", h, "_", l));
		}
	}
	int const R1(columnBuffer.size());
	columnBuffer.add(0, continuous, -infinity, +infinity, "R1");

	int const B1(columnBuffer.size());
	columnBuffer.add(0, continuous, -infinity, +infinity, "B1");
	//_solver->add(columnBuffer);
	// binary declaration

	// constraints
	std::vector<double> rhs;
	std::vector<char> rtype;
	std::vector<int> rmatbeg;
	std::vector<int> rmatind;
	std::vector<double> rmatval;
	std::vector<std::string> rname;
	rowBuffer.clear();
	// R1 = sum kr Yr
	rowBuffer.add(0, 'E', "R1_k");
	for (int r(0); r < _biPartiteGraph->nR(); ++r) {
		rowBuffer.add(r, _biPartiteGraph->k(r));
	}
	rowBuffer.add(R1, -1);
	// R1 = sum 2^h ah
	rowBuffer.add(0, 'E', "R1_POW");
	for (int h(0); h < tR + 1; ++h) {
		rowBuffer.add(a[h], std::pow(2, h));
	}
	rowBuffer.add(R1, -1);
	// B1 = sum kb Yb
	rowBuffer.add(0, 'E', "B1_k");
	for (int b(0); b < _biPartiteGraph->nB(); ++b) {
		rowBuffer.add(_biPartiteGraph->nR() + b,
			_biPartiteGraph->k(_biPartiteGraph->nR() + b));
	}
	rowBuffer.add(B1, -1);
	// B1 = sum 2^l al
	rowBuffer.add(0, 'E', "B1_POW");
	for (int l(0); l < tB + 1; ++l) {
		rowBuffer.add(b[l], std::pow(2, l));
	}
	rowBuffer.add(B1, -1);
	for (int r(0); r < _biPartiteGraph->nR(); ++r) {
		for (int b(0); b < _biPartiteGraph->nB(); ++b) {
			if (_biPartiteGraph->a(r, b) != 0) {
				// Srb <= Yr (r,b) in E
				rowBuffer.add(0, 'L', GetStr("S_LEQ_R_R", r, "_B", b));
				rowBuffer.add(r, -1);
				rowBuffer.add((int)_s.get(r, b), +1);
				// Srb <= Yb (r,b) in E
				rowBuffer.add(0, 'L', GetStr("S_LEQ_B_R", r, "_B", b));
				rowBuffer.add(_biPartiteGraph->nR() + b, -1);
				rowBuffer.add((int)_s.get(r, b), +1);
				//				// Srb >= Yr+ Yb -1  (r,b) in E
				//				_rowBuffer.add(-1, 'G', GetStr("SYRB_", r, "_", b));
				//				_rowBuffer.add(r, -1);
				//				_rowBuffer.add(_biPartiteGraph->nR() + b, -1);
				//				_rowBuffer.add((int) _s.get(r, b), +1);
			}
		}
	}
	// abhl >= ah+bl-1
	for (int h(0); h < tR + 1; ++h) {
		for (int l(0); l < tB + 1; ++l) {
			rowBuffer.add(-1, 'G', GetStr("FORTET_", h, "_", l));
			rowBuffer.add((int)_ab.get(h, l), 1);
			rowBuffer.add(a[h], -1);
			rowBuffer.add(b[l], -1);
		}
	}
	//	// abhl <= ah
	//	for (int h(0); h < tR + 1; ++h) {
	//		for (int l(0); l < tB + 1; ++l) {
	//			// abhl <= ah
	//			_rowBuffer.add(0, 'L', GetStr("abh_", h, "_", l));
	//			_rowBuffer.add((int) _ab.get(h, l), 1);
	//			_rowBuffer.add(a[h], -1);
	//			// abhl <= bl
	//			_rowBuffer.add(0, 'L', GetStr("abl_", h, "_", l));
	//			_rowBuffer.add((int) _ab.get(h, l), 1);
	//			_rowBuffer.add(b[l], -1);
	//		}
	//	}
	//_solver->add(_rowBuffer);
}
void BinaryDecompositionOracle::initOracle() {
	DoubleVector v;
	std::ofstream file("q.txt");
	for (int r(0); r < _biPartiteGraph->nR(); ++r) {
		for (int b(0); b < _biPartiteGraph->nB(); ++b) {
			double const value(_biPartiteGraph->w(r, b));
			file << std::setw(6) << r;
			file << std::setw(6) << _biPartiteGraph->nR() +b;
			file << std::setw(50) << std::setprecision(25) << value;
			file << std::endl;
		}
	}
	file.close();
  _solver->initLp("Oracle");
  ColumnBuffer columnBuffer(_solver->continuous());
  fill(_rowBuffer, columnBuffer);
  _solver->add(columnBuffer);
  _solver->add(_rowBuffer);
  _solver->maximize();
}

void BinaryDecompositionOracle::checkSolutions() {
  throw std::invalid_argument("");
//	CpxOracle::checkSolutions();
//	DoubleVector x(CPXgetnumcols(_env, _prob));
//	int const n(1 + 0 * CPXgetsolnpoolnumsolns(_env, _prob));
//	//			std::cout << std::setw(4) << n << std::endl;
//	Double obj;
//	for (int i(0); i < n; ++i) {
//		CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
//		CPXgetsolnpoolx(_env, _prob, (int) i, x.data(), 0,
//				(int) (x.size() - 1));
//		Column column(_biPartiteGraph);
//
//		for (int v(0); v < _biPartiteGraph->nV(); ++v) {
//			if (x[v] > 0.5) {
//				column.insert(v);
//			}
//		}
//		for (int r(0); r < _biPartiteGraph->nR(); ++r) {
//			for (int b(0); b < _biPartiteGraph->nB(); ++b) {
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

