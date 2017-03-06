#include "MilpOracle.h"

MilpOracle::MilpOracle(CliquePartitionProblem const * input)
    : CpxOracle(input),
      _cpp(input) {
  initCpx();
}
MilpOracle::~MilpOracle() {
}


void MilpOracle::fill(RowBuffer & rowBuffer, ColumnBuffer & columnBuffer) {

	char const binary(_solver->binary());
	char const continuous(_solver->continuous());
	double const infinity(_solver->infinity());
	char const geq(_solver->geq());
	char const eq(_solver->eq());
	char const leq(_solver->leq());

	_index.resize(_input->nV());
	for (int v(0); v < _input->nV(); ++v) {
		_index[v] = columnBuffer.size();
		columnBuffer.add(0, binary, 0, 1, _cpp->name(v));
	}

	// Srb=Yr.Yb
	std::vector<Edge> const & costs(_cpp->getCosts());
	_products.assign(costs.size(), Product());
	int i(0);
	for (auto const & edge : costs) {
		_products[i]._first = edge._i;
		_products[i]._second = edge._j;
		_products[i]._firstTimeSecond = columnBuffer.size();
		_products[i]._coeff = edge._v;
		columnBuffer.add(edge._v, continuous,edge._v < 0 ? 0 : -infinity,infinity,GetStr("PRODUCT_", _cpp->name(edge._i), "_",_cpp->name(edge._j)));
		++i;
	}
	//	_s = RectMatrix(_input->nR(), _input->nB());
	//	for (int r(0); r < _input->nR(); ++r) {
	//		for (int b(0); b < _input->nB(); ++b) {
	//			if (_input->w(r, b) != 0) {
	//				_s.get(r, b) = columnBuffer.size();
	//				columnBuffer.add(_input->w(r, b), CPX_CONTINUOUS,
	//						_input->w(r, b) < 0 ? 0 : -CPX_INFBOUND,
	//						CPX_INFBOUND, GetStr("S_", r, "_", b));
	//			}
	//		}
	//	}

	// binary declaration

	// constraints
	std::vector<double> rhs;
	std::vector<char> rtype;
	std::vector<int> rmatbeg;
	std::vector<int> rmatind;
	std::vector<double> rmatval;
	std::vector<std::string> rname;
	
	rowBuffer.clear();
	for (auto const & product : _products) {
		std::string const & firstName(_cpp->name(product._first));
		std::string const & secondName(_cpp->name(product._second));
		std::string const & productName(firstName + "_" + secondName);
		int const r(product._first);
		int const b(product._second);
		if (product._coeff > 0) {
			// Srb <= Yr (r,b) in RxB
			rowBuffer.add(0, leq, GetStr(productName, "_LEQ_", firstName));
			rowBuffer.add(r, -1);
			rowBuffer.add(product._firstTimeSecond, +1);
			// Srb <= Yb (r,b) in RxB
			rowBuffer.add(0, leq, GetStr(productName, "_LEQ_", secondName));
			rowBuffer.add(b, -1);
			rowBuffer.add(product._firstTimeSecond, +1);
		}
		else if (product._coeff < 0) {
			// Srb >= 0 already in
			// Srb >= Yr+Yb-1
			rowBuffer.add(-1, geq, GetStr(productName, "_GEQ_", firstName, "_", secondName));
			rowBuffer.add(r, -1);
			rowBuffer.add(b, -1);
			rowBuffer.add(product._firstTimeSecond, +1);
		}
	}
}

void MilpOracle::initOracle() {
	_solver->initLp("MilpOracle");
	char const continuous(_solver->continuous());
	// 0..R-1 : Yr
	// R..R+B-1 : Yb
	ColumnBuffer columnBuffer(continuous);
	fill(_rowBuffer, columnBuffer);
	_solver->add(columnBuffer);
	_solver->add(_rowBuffer	);
	_solver->maximize();
//	CPXwriteprob(_env, _prob, "MILP.lp", "LP");
//	CPXmipopt(_env, _prob);
//	double toto;
//	CPXgetobjval(_env, _prob, &toto);
//	std::cout << "toto : " << toto << std::endl;
//	std::exit(0);
}

void MilpOracle::setUpOracle() {
	_columns.clear();
	_solver->chgObj(_index, *_minus_dual);
	//	for (auto const & i : _index) {
	//		std::cout << std::setw(6) << i;
	//		std::cout << std::setw(25) << _dual->at(i);
	//		std::cout << std::endl;
	//	}
	//	write();
	_solver->delMipStarts();
}

void MilpOracle::checkMipSolution() const {
//	CpxOracle::checkSolutions();
}

