#include "MipGenerator.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"

#include <cplex.h>

MipGenerator::MipGenerator(BipartiteGraph const &input,
		DoubleVector const & dual, DecisionList const & decisions) :
		_env(NULL), _oracle(NULL), _input(&input), _dual(&dual), _columns(), _index(), _bestReducedCost(
				0), _decisions(&decisions) {
	initCpx();
}
MipGenerator::MipGenerator(BipartiteGraph const * input,
		DoubleVector const * dual, DecisionList const * decisions) :
		_env(NULL), _oracle(NULL), _input(input), _dual(dual), _columns(), _index(), _bestReducedCost(
				0), _decisions(decisions) {
	initCpx();
}
void MipGenerator::initCpx() {
	int err;
	_env = CPXopenCPLEX(&err);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	//	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
	CPXsetintparam(_env, CPX_PARAM_CUTPASS, -1);
	CPXsetintparam(_env, CPX_PARAM_VARSEL, 4);
	CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY, 2);
	initOracle();

}
MipGenerator::~MipGenerator() {
	freeLp();
}
void MipGenerator::freeLp() {
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
	if (_oracle != NULL) {
		CPXfreeprob(_env, &_oracle);
		_oracle = NULL;
	}
}
void MipGenerator::initOracle() {
	int err(0);
	if (_oracle != NULL) {
		CPXfreeprob(_env, &_oracle);
		_oracle = NULL;
	}
	_oracle = CPXcreateprob(_env, &err, "Oracle");
	std::vector<double> lb;
	std::vector<double> ub;
	std::vector<int> cindex;
	std::vector<char> ctype;
	std::vector<double> obj;
	_cname.clear();
	// 0..R-1 : Yr
	// R..R+B-1 : Yb
	size_t id(0);
	for (; id < _input->nV(); ++id) {
		// Y binary
		cindex.push_back((int) id);
		ctype.push_back(CPX_BINARY);
		lb.push_back(+0);
		ub.push_back(+1);
		obj.push_back(0);
		if (id < _input->nR())
			_cname.push_back(GetStr("YR_", id));
		else
			_cname.push_back(GetStr("YB_", id - _input->nR()));
	}
	_index = cindex;
	// 
	_s = RectMatrix(_input->nR(), _input->nB());
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			// S reel
			_s.get(r, b) = (int) id;
			cindex.push_back((int) id);
			ctype.push_back(CPX_CONTINUOUS);
			//		lb.push_back(-CPX_INFBOUND);
			lb.push_back(-0);
			ub.push_back(+1);
			if (_input->a(r, b) != 0)
				obj.push_back(_input->a(r, b) * _input->inv_m());
			else
				obj.push_back(0);
			_cname.push_back(GetStr("S_", r, "_", b));
			++id;
		}
	}
	size_t const tR(
			(size_t) std::ceil((std::log(_input->sum_kR() + 1) / log(2) - 1)));
	size_t const tB(
			(size_t) std::ceil((std::log(_input->sum_kB() + 1) / log(2) - 1)));
	IntVector a(tR + 1);
	for (size_t h(0); h < tR + 1; ++h, ++id) {
		// ah binary
		a[h] = id;
		cindex.push_back((int) id);
		ctype.push_back(CPX_BINARY);
		lb.push_back(+0);
		ub.push_back(+1);
		obj.push_back(0);
		_cname.push_back(GetStr("a_", h));
	}
	IntVector b(tR + 1);
	for (size_t l(0); l < tB + 1; ++l, ++id) {
		// bl binary
		b[l] = id;
		cindex.push_back((int) id);
		ctype.push_back(CPX_BINARY);
		lb.push_back(+0);
		ub.push_back(+1);
		obj.push_back(0);
		_cname.push_back(GetStr("b_", l));
	}
	RectMatrix ab(tR + 1, tB + 1);
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l, ++id) {
			// ahl >= 0
			ab.get(h, l) = (int) id;
			cindex.push_back((int) id);
			ctype.push_back(CPX_CONTINUOUS);
			lb.push_back(+0);
			ub.push_back(+CPX_INFBOUND);
			obj.push_back(
					-std::pow(2, l + h) * _input->inv_m() * _input->inv_m());
			_cname.push_back(GetStr("a_", h, "_", l));
		}
	}
	size_t const R1(id);
	cindex.push_back((int) id);
	ctype.push_back(CPX_CONTINUOUS);
	lb.push_back(-CPX_INFBOUND);
	ub.push_back(+CPX_INFBOUND);
	obj.push_back(0);
	_cname.push_back("R1");
	++id;
	size_t const B1(id);
	cindex.push_back((int) id);
	ctype.push_back(CPX_CONTINUOUS);
	lb.push_back(-CPX_INFBOUND);
	ub.push_back(+CPX_INFBOUND);
	obj.push_back(0);
	_cname.push_back("B1");
	++id;
	size_t ccnt(id);
	std::vector<int> cmatbeg(ccnt + 1, 0);
	//CPXsetintparam(_env, CPX_PARAM_THREADS, 0);
	// variables
	std::vector<char*> cnameCPX(_cname.size());
	for (size_t i(0); i < _cname.size(); ++i) {
		cnameCPX[i] = const_cast<char*>(_cname[i].c_str());
	}
	CPXaddcols(_env, _oracle, (int) ccnt, 0, obj.data(), cmatbeg.data(), NULL,
			NULL, lb.data(), ub.data(),
			//NULL
			cnameCPX.data());
	// binary declaration
	CPXchgctype(_env, _oracle, (int) cindex.size(), cindex.data(),
			ctype.data());

	// constraints
	std::vector<double> rhs;
	std::vector<char> rtype;
	std::vector<int> rmatbeg;
	std::vector<int> rmatind;
	std::vector<double> rmatval;
	std::vector<std::string> rname;
	// R1 = sum kr Yr
	rmatbeg.push_back((int) rmatval.size());
	rname.push_back("R1_k");
	for (size_t r(0); r < _input->nR(); ++r) {
		rmatind.push_back((int) r);
		rmatval.push_back(_input->kR(r));
	}
	rmatind.push_back((int) R1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// R1 = sum 2^h ah
	rmatbeg.push_back((int) rmatval.size());
	rname.push_back("R1_POW");
	for (size_t h(0); h < tR + 1; ++h) {
		rmatind.push_back((int) a[h]);
		rmatval.push_back(std::pow(2, h));
	}
	rmatind.push_back((int) R1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// B1 = sum kb Yb
	rmatbeg.push_back((int) rmatval.size());
	rname.push_back("B1_k");
	for (size_t b(0); b < _input->nB(); ++b) {
		rmatind.push_back((int) (_input->nR() + b));
		rmatval.push_back(_input->kB(b));
	}
	rmatind.push_back((int) B1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// B1 = sum 2^l al
	rmatbeg.push_back((int) rmatval.size());
	rname.push_back("B1_POW");
	for (size_t l(0); l < tB + 1; ++l) {
		rmatind.push_back((int) b[l]);
		rmatval.push_back(std::pow(2, l));
	}
	rmatind.push_back((int) B1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// Srb <= Yr (r,b) in E
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			rmatbeg.push_back((int) rmatval.size());
			rname.push_back(GetStr("SY_", r));
			rmatind.push_back((int) r);
			rmatval.push_back(-1);
			rmatind.push_back((int) _s.get(r, b));
			rmatval.push_back(+1);
			rtype.push_back('L');
			rhs.push_back(0);
		}
	}
	// Srb <= Yb (r,b) in E
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			rmatbeg.push_back((int) rmatval.size());
			rname.push_back(GetStr("SY_", b));
			rmatind.push_back((int) (_input->nR() + b));
			rmatval.push_back(-1);
			rmatind.push_back((int) _s.get(r, b));
			rmatval.push_back(+1);
			rtype.push_back('L');
			rhs.push_back(0);
		}
	}
	// Srb >= Yr+ Yb -1  (r,b) in E
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			rmatbeg.push_back((int) rmatval.size());
			rname.push_back(GetStr("SYRB_", r, "_", b));
			rmatind.push_back((int) (r));
			rmatval.push_back(-1);
			rmatind.push_back((int) (_input->nR() + b));
			rmatval.push_back(-1);
			rmatind.push_back((int) _s.get(r, b));
			rmatval.push_back(+1);
			rtype.push_back('G');
			rhs.push_back(-1);
		}
	}
	// abhl >= ah+bl-1
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			rmatbeg.push_back((int) rmatval.size());
			rname.push_back(GetStr("FORTET_", h, "_", l));
			rmatind.push_back((int) ab.get(h, l));
			rmatval.push_back((int) +1);
			rmatind.push_back((int) a[h]);
			rmatval.push_back(-1);
			rmatind.push_back((int) b[l]);
			rmatval.push_back(-1);
			rtype.push_back('G');
			rhs.push_back(-1);
		}
	}
	// abhl <= ah
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			rmatbeg.push_back((int) rmatval.size());
			rname.push_back(GetStr("abh_", h, "_", l));
			rmatind.push_back((int) ab.get(h, l));
			rmatval.push_back((int) +1);
			rmatind.push_back((int) a[h]);
			rmatval.push_back(-1);
			rtype.push_back('L');
			rhs.push_back(0);
		}
	}
	// abhl <= bl
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			rmatbeg.push_back((int) rmatval.size());
			rname.push_back(GetStr("abl_", h, "_", l));
			rmatind.push_back((int) ab.get(h, l));
			rmatval.push_back((int) +1);
			rmatind.push_back((int) b[l]);
			rmatval.push_back(-1);
			rtype.push_back('L');
			rhs.push_back(0);
		}
	}
	std::vector<char*> rnameCPX(rname.size());
	for (size_t i(0); i < rname.size(); ++i) {
		rnameCPX[i] = const_cast<char*>(rname[i].c_str());
	}
	rmatbeg.push_back((int) rmatval.size());
	CPXaddrows(_env, _oracle, 0, (int) rhs.size(), (int) rmatval.size(),
			rhs.data(), rtype.data(), rmatbeg.data(), rmatind.data(),
			rmatval.data(), NULL, rnameCPX.data());
//	CPXaddlazyconstraints(_env, _oracle, (int) rhs.size(), (int) rmatval.size(),
//			rhs.data(), rtype.data(), rmatbeg.data(), rmatind.data(),
//			rmatval.data(), rnameCPX.data());
	CPXchgobjsen(_env, _oracle, -1);
	//writeOracle();
	//CPXmipopt(_env, _oracle);
	//double toto;
	//CPXgetobjval(_env, _oracle, &toto);
	//std::cout << "toto : "<<toto<<std::endl;

	_index3.clear();
	_index2.clear();
	for (size_t r(0); r < _input->nR(); ++r) {
		for (size_t b(0); b < _input->nB(); ++b) {
			_index2[std::make_pair(r, b)] = _index3.size();
			_index3.push_back((int) _s.get(r, b));
		}
	}

}

// on initialise avec les singletons
void MipGenerator::write(std::string const & fileName) const {
	CPXwriteprob(_env, _oracle, fileName.c_str(), "LP");
}

void MipGenerator::setUpOracle() {
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
Double MipGenerator::bestReducedCost() const {
	return _bestReducedCost;
}

bool MipGenerator::generate() {
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
					//					std::cout << std::setw(4) << i;
					//					std::cout << std::setw(30) << std::setprecision(20) << obj
					//							<< std::endl;
					CPXgetsolnpoolx(_env, _oracle, (int) i, x.data(), 0,
							(int) (x.size() - 1));
					Column column(_input);
					//					for (size_t i(0); i < _input->nV(); ++i) {
					//						std::cout << std::setw(4) << i;
					//						std::cout << std::setw(15) << x[i];
					//						std::cout << std::endl;
					//					}
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
					write();
					assert(column.violation(*_decisions) == 0);
					_columns.insert(column);

				}
			}

			//CPXgetx (_env, _oracle, x.data(), 0, x.size()-1);
			// verification
			//DoubleVector obj(_input->nV()+_input->nR()*_input->nB());
			//CPXgetobj(_env, _oracle, obj.data(), 0, obj.size()-1);
			//Double cost(0);
			//for(size_t r(0); r<_input->nR(); ++r){
			//	size_t const yR(r);
			//	for(size_t b(0); b<_input->nB(); ++b){
			//		size_t const yB(_input->nR()+b);
			//		size_t const yRB(_input->nV()+r*_input->nB()+b);
			//		assert( std::fabs( x[yR]*x[yB]-x[yRB] )<1e-10);
			//		cost+= x[yRB] * obj[yRB];
			//	}
			//}
			//Double reducedCost(cost);
			//Column column(_input);
			//for(size_t r(0); r<_input->nR(); ++r){
			//	if (x[r]>0.5){
			//		column.r().insert(r);
			//	}
			//}
			//for(size_t b(0); b<_input->nB(); ++b){
			//	if (x[_input->nR()+b]>0.5){
			//		column.b().insert(b);
			//	}
			//}
			//column.computeCost();
			//column.reducedCost() = _bestReducedCost;
			//_columns.insert(column);
		}
	} else
		std::cout << "CPXgetstat : " << CPXgetstat(_env, _oracle) << std::endl;
	return result;
}

std::set<Column>
const & MipGenerator::columns() const {
	return _columns;
}

void MipGenerator::applyBranchingRule() {
	std::vector<double> lb(_index3.size(), 0);
	std::vector<double> ub(_index3.size(), 1);
	std::vector<char> lc(_index3.size(), 'L');
	std::vector<char> uc(_index3.size(), 'U');
	applyBranchingRule(_index3, lb, ub);

	CPXchgbds(_env, _oracle, (int) _index3.size(), _index3.data(), lc.data(),
			lb.data());
	CPXchgbds(_env, _oracle, (int) _index3.size(), _index3.data(), uc.data(),
			ub.data());
}
void MipGenerator::applyBranchingRule(std::vector<int> & index,
		std::vector<double> & lb, std::vector<double> & ub) {
	for (Decision const & decision : *_decisions) {
		if (decision.cannot()) {
			ub[_index2[std::make_pair(decision.r(), decision.b())]] = 0;
		} else {

			lb[_index2[std::make_pair(decision.r(), decision.b())]] = 1;
		}
	}

}
