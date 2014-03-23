#include "BinaryDecompositionOracle.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"

#include <cplex.h>

BinaryDecompositionOracle::BinaryDecompositionOracle(
		BipartiteGraph const * input, DoubleVector const * dual,
		DecisionList const * decisions) :
		IOracle(input, dual, decisions), _env(NULL), _oracle(NULL), _index() {
	initCpx();
}
void BinaryDecompositionOracle::initCpx() {
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
BinaryDecompositionOracle::~BinaryDecompositionOracle() {
	freeLp();
}
void BinaryDecompositionOracle::freeLp() {
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
	if (_oracle != NULL) {
		CPXfreeprob(_env, &_oracle);
		_oracle = NULL;
	}
}
void BinaryDecompositionOracle::initOracle() {
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

	// 
	RectMatrix s = RectMatrix(_input->nR(), _input->nB());
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			if (_input->a(r, b) != 0) {
				s.get(r, b) = columnBuffer.size();
				columnBuffer.add(_input->a(r, b) * _input->inv_m(),
						CPX_CONTINUOUS, -CPX_INFBOUND, CPX_INFBOUND,
						GetStr("S_", r, "_", b));
			}
		}
	}
	size_t const tR(
			(size_t) std::ceil((std::log(_input->sum_kR() + 1) / log(2) - 1)));
	size_t const tB(
			(size_t) std::ceil((std::log(_input->sum_kB() + 1) / log(2) - 1)));
	IntVector a(tR + 1);
	for (size_t h(0); h < tR + 1; ++h) {
		// ah binary
		a[h] = columnBuffer.size();
		columnBuffer.add(0, CPX_BINARY, 0, 1, GetStr("a_", h));
	}
	IntVector b(tR + 1);
	for (size_t l(0); l < tB + 1; ++l) {
		// bl binary
		b[l] = columnBuffer.size();
		columnBuffer.add(0, CPX_BINARY, 0, 1, GetStr("b_", l));
	}
	RectMatrix ab(tR + 1, tB + 1);
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			// ahl >= 0
			ab.get(h, l) = columnBuffer.size();
			columnBuffer.add(
					-std::pow(2, l + h) * _input->inv_m() * _input->inv_m(),
					CPX_CONTINUOUS, 0, CPX_INFBOUND, GetStr("a_", h, "_", l));
		}
	}
	size_t const R1(columnBuffer.size());
	columnBuffer.add(0, CPX_CONTINUOUS, -CPX_INFBOUND, +CPX_INFBOUND, "R1");
	_cname.push_back("R1");

	size_t const B1(columnBuffer.size());
	columnBuffer.add(0, CPX_CONTINUOUS, -CPX_INFBOUND, +CPX_INFBOUND, "B1");

	columnBuffer.add(_env, _oracle);
	// binary declaration

	// constraints
	std::vector<double> rhs;
	std::vector<char> rtype;
	std::vector<int> rmatbeg;
	std::vector<int> rmatind;
	std::vector<double> rmatval;
	std::vector<std::string> rname;
	_rowBuffer.clear();
	// R1 = sum kr Yr
	_rowBuffer.add(0, 'E', "R1_k");
	for (size_t r(0); r < _input->nR(); ++r) {
		_rowBuffer.add(r, _input->kR(r));
	}
	_rowBuffer.add(R1, -1);
	// R1 = sum 2^h ah
	_rowBuffer.add(0, 'E', "R1_POW");
	for (size_t h(0); h < tR + 1; ++h) {
		_rowBuffer.add(a[h], std::pow(2, h));
	}
	_rowBuffer.add(R1, -1);
	// B1 = sum kb Yb
	_rowBuffer.add(0, 'E', "B1_k");
	for (size_t b(0); b < _input->nB(); ++b) {
		_rowBuffer.add(_input->nR() + b, _input->kB(b));
	}
	_rowBuffer.add(B1, -1);
	// B1 = sum 2^l al
	_rowBuffer.add(0, 'E', "B1_POW");
	for (size_t l(0); l < tB + 1; ++l) {
		_rowBuffer.add(b[l], std::pow(2, l));
	}
	_rowBuffer.add(B1, -1);
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			if (_input->a(r, b) != 0) {
				// Srb <= Yr (r,b) in E
				_rowBuffer.add(0, 'L', GetStr("SYR_", r));
				_rowBuffer.add(r, -1);
				_rowBuffer.add((int) s.get(r, b), +1);
				// Srb <= Yb (r,b) in E
				_rowBuffer.add(0, 'L', GetStr("SYB_", b));
				_rowBuffer.add(_input->nR() + b, -1);
				_rowBuffer.add((int) s.get(r, b), +1);
			}
		}
	}
//	// Srb >= Yr+ Yb -1  (r,b) in E
//	for (size_t r(0); r < _input->nR(); ++r) {
//		for (size_t b(0); b < _input->nB(); ++b) {
//			if (_input->a(r, b) != 0) {
//				_rowBuffer.add(-1, 'G', GetStr("SYRB_", r, "_", b));
//				_rowBuffer.add(r, -1);
//				_rowBuffer.add(_input->nR() + b, -1);
//				_rowBuffer.add((int) _s.get(r, b), +1);
//			}
//		}
//	}
	// abhl >= ah+bl-1
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			_rowBuffer.add(-1, 'G', GetStr("FORTET_", h, "_", l));
			_rowBuffer.add((int) ab.get(h, l), 1);
			_rowBuffer.add(a[h], -1);
			_rowBuffer.add(b[l], -1);
		}
	}
	// abhl <= ah
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			// abhl <= ah
			_rowBuffer.add(0, 'L', GetStr("abh_", h, "_", l));
			_rowBuffer.add((int) ab.get(h, l), 1);
			_rowBuffer.add(a[h], -1);
			// abhl <= bl
			_rowBuffer.add(0, 'L', GetStr("abl_", h, "_", l));
			_rowBuffer.add((int) ab.get(h, l), 1);
			_rowBuffer.add(b[l], -1);
		}
	}
	_rowBuffer.add(_env, _oracle);
	CPXchgobjsen(_env, _oracle, -1);
	//writeOracle();
	//CPXmipopt(_env, _oracle);
	//double toto;
	//CPXgetobjval(_env, _oracle, &toto);
	//std::cout << "toto : "<<toto<<std::endl;
}

// on initialise avec les singletons
void BinaryDecompositionOracle::write(std::string const & fileName) const {
	CPXwriteprob(_env, _oracle, fileName.c_str(), "LP");
}

void BinaryDecompositionOracle::setUpOracle() {
	_columns.clear();
	CPXchgobj(_env, _oracle, (int) _index.size(), _index.data(), _dual->data());

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
bool BinaryDecompositionOracle::generate() {
	setUpOracle();
	CPXmipopt(_env, _oracle);
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
//				std::cout << "_bestReducedCost : " << _bestReducedCost << std::endl;
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

void BinaryDecompositionOracle::applyBranchingRule() {
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
