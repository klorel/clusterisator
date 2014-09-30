#include "MilpOracle.hpp"

#include <cplex.h>

MilpOracle::MilpOracle(ICliquePartitionProblem const * input,
		DoubleVector const * dual, DecisionList const * decisions) :
		CpxOracle(input, dual, decisions) {
	initCpx();
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
		columnBuffer.add(0, CPX_BINARY, 0, 1, _input->name(v));
	}

	// Srb=Yr.Yb
	std::vector<Edge> const & costs(_input->costs());
	_products.assign(costs.size(), Product());
	size_t i(0);
	for (auto const & edge : costs) {
		_products[i]._first = edge._i;
		_products[i]._second = edge._j;
		_products[i]._firstTimeSecond = columnBuffer.size();
		_products[i]._coeff = edge._v;
		columnBuffer.add(edge._v, CPX_CONTINUOUS,
				edge._v < 0 ? 0 : -CPX_INFBOUND,
						CPX_INFBOUND,
						GetStr("PRODUCT_", _input->name(edge._i), "_",
								_input->name(edge._j)));
	}
	//	_s = RectMatrix(_input->nR(), _input->nB());
	//	for (size_t r(0); r < _input->nR(); ++r) {
	//		for (size_t b(0); b < _input->nB(); ++b) {
	//			if (_input->w(r, b) != 0) {
	//				_s.get(r, b) = columnBuffer.size();
	//				columnBuffer.add(_input->w(r, b), CPX_CONTINUOUS,
	//						_input->w(r, b) < 0 ? 0 : -CPX_INFBOUND,
	//						CPX_INFBOUND, GetStr("S_", r, "_", b));
	//			}
	//		}
	//	}
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
	for (auto const & product : _products) {
		std::string const & firstName(_input->name(product._first));
		std::string const & secondName(_input->name(product._second));
		std::string const & productName(firstName + "_" + secondName);
		size_t const r(product._first);
		size_t const b(product._second);
		if (product._coeff > 0) {
			// Srb <= Yr (r,b) in RxB
			_rowBuffer.add(0, 'L', GetStr(productName, "_LEQ_", firstName));
			_rowBuffer.add(r, -1);
			_rowBuffer.add(product._firstTimeSecond, +1);
			// Srb <= Yb (r,b) in RxB
			_rowBuffer.add(0, 'L', GetStr(productName, "_LEQ_", secondName));
			_rowBuffer.add(b, -1);
			_rowBuffer.add(product._firstTimeSecond, +1);
		} else if (product._coeff < 0) {
			// Srb >= 0 already in
			// Srb >= Yr+Yb-1
			_rowBuffer.add(-1, 'G',
					GetStr(productName, "_GEQ_", firstName, "_", secondName));
			_rowBuffer.add(r, -1);
			_rowBuffer.add(b, -1);
			_rowBuffer.add(product._firstTimeSecond, +1);
		}
	}

	_rowBuffer.add(_env, _prob);
	CPXchgobjsen(_env, _prob, -1);
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
		CPXdelmipstarts(_env, _prob, 0, CPXgetnummipstarts(_env, _prob) - 1);
}

void MilpOracle::checkMipSolution() const {
	Double obj;
	DoubleVector x(CPXgetnumcols(_env, _prob));
	size_t const n(CPXgetsolnpoolnumsolns(_env, _prob));
	for (size_t i(0); i < n; ++i) {
		CPXgetsolnpoolobjval(_env, _prob, (int) i, &obj);
		std::cout << "SOLUTION" << std::setw(2) << i << std::setw(10) << obj
				<< std::endl;

		for (auto const & product : _products) {
			size_t const r(product._first);
			size_t const b(product._second);
			double const yR(x[r]);
			double const yB(x[b]);
			double const sRB(x[product._firstTimeSecond]);
			//				std::cout << std::setw(4) << yR;
			//				std::cout << std::setw(4) << yB;
			//				std::cout << std::setw(4) << sRB;
			//				std::cout << std::endl;
			assert(std::abs(yR * yB - sRB) < 1e-10);
		}
	}
}

