#include "Divisor.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"

#include "LpBuffer.hpp"

#include <cplex.h>

Divisor::Divisor(BipartiteGraph const &input) :
		_env(NULL), _lp(NULL), _input(&input), _y() {
	initCpx();
}
Divisor::Divisor(BipartiteGraph const * input) :
		_env(NULL), _lp(NULL), _input(input), _y() {
	initCpx();
}
void Divisor::initCpx() {
	int err;
	_env = CPXopenCPLEX(&err);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
//	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
	CPXsetintparam(_env, CPX_PARAM_CUTPASS, -1);
	CPXsetintparam(_env, CPX_PARAM_VARSEL, 4);
	CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY, 2);
}
Divisor::~Divisor() {
	freeLp();
}
void Divisor::freeLp() {
	if (_lp != NULL) {
		CPXfreeprob(_env, &_lp);
		_lp = NULL;
	}
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
}
Double Divisor::buildLp(ModularityBPartition const & partition, size_t label) {
	Double Rc(0);
	for (size_t r(0); r < _input->nR(); ++r) {
		if (partition.label(r) == label)
			Rc += _input->k(r);
	}
	Double Bc(0);
	for (size_t b(0); b < _input->nB(); ++b) {
		if (partition.label(_input->nR() + b) == label)
			Bc += _input->k(_input->nR() + b);
	}
	Double Ec(0);
	for (Edge const & edge : _input->edges()) {
		if (partition.label(_input->nR() + edge._j) == label
				&& partition.label(edge._i) == label)
			++Ec;
	}
	int err(0);
	if (_lp != NULL) {
		CPXfreeprob(_env, &_lp);
		_lp = NULL;
	}
	Double const cst_obj(_input->inv_m() * (Ec - Rc * Bc * _input->inv_m()));
//	MY_PRINT(Rc);
//	MY_PRINT(Bc);
//	MY_PRINT(Ec);
//	MY_PRINT(_input->inv_m());
//	MY_PRINT(Rc * Bc * _input->inv_m());
//	MY_PRINT(cst_obj);

	_lp = CPXcreateprob(_env, &err, "Divisor");
	ColumnBuffer columns;
	// 0..R-1 : Yr
	// R..R+B-1 : Yb
	_y.clear();
	_y.resize(_input->nV(), -1);
	for (size_t v(0); v < _input->nV(); ++v) {
		if (partition.label(v) == label) {
			// Y binary
			_y[v] = columns.size();
			columns.add(0, CPX_BINARY, 0, 1,
					v < _input->nR() ?
							GetStr("YR_", v) : GetStr("YB_", v - _input->nR()));

		}
	}
	// 
	RectMatrix s(_input->nR(), _input->nB());
	for (Edge const & edge : _input->edges()) {
		if (partition.label(_input->nR() + edge._j) == label
				&& partition.label(edge._i) == label) {
			// S reel
			s.get(edge._i, edge._j) = columns.size();
			columns.add(2 * _input->inv_m(), CPX_CONTINUOUS, -CPX_INFBOUND,
					+CPX_INFBOUND, GetStr("S_", edge._i, "_", edge._j));
			columns.rhsObj(_y[edge._i]) -= _input->inv_m();
			columns.rhsObj(_y[_input->nR() + edge._j]) -= _input->inv_m();
		}
	}

	size_t const tR((size_t) std::ceil((std::log(Rc + 1) / log(2) - 1)));
	size_t const tB((size_t) std::ceil((std::log(Bc + 1) / log(2) - 1)));
	IntVector a(tR + 1);
	for (size_t h(0); h < tR + 1; ++h) {
		// ah binary
		a[h] = columns.size();
		columns.add(0, CPX_BINARY, 0, 1, GetStr("a_", h));
	}
	IntVector b(tB + 1);
	for (size_t l(0); l < tB + 1; ++l) {
		// bl binary
		b[l] = columns.size();
		columns.add(0, CPX_BINARY, 0, 1, GetStr("b_", l));
	}
	RectMatrix ab(tR + 1, tB + 1);
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			// ahl >= 0
			ab.get(h, l) = columns.size();
			columns.add(
					-std::pow(2, 1 + l + h) * _input->inv_m() * _input->inv_m(),
					CPX_CONTINUOUS, 0, CPX_INFBOUND, GetStr("a_", h, "_", l));

		}
	}
	size_t const R1(columns.size());
	columns.add(+Bc * _input->inv_m() * _input->inv_m(), CPX_CONTINUOUS,
			-CPX_INFBOUND, +CPX_INFBOUND, "R1");

	size_t const B1(columns.size());
	columns.add(+Rc * _input->inv_m() * _input->inv_m(), CPX_CONTINUOUS,
			-CPX_INFBOUND, +CPX_INFBOUND, "B1");

	columns.add(_env, _lp);

// constraints
	RowBuffer rows;
// R1 = sum kr Yr
	rows.add(0, 'E', "R1_k");
	for (size_t r(0); r < _input->nR(); ++r) {
		if (partition.label(r) == label) {
			rows.add(_y[r], _input->k(r));
		}
	}
	rows.add(R1, -1);
// R1 = sum 2^h ah
	rows.add(0, 'E', "R1_POW");
	for (size_t h(0); h < tR + 1; ++h) {
		rows.add(a[h], std::pow(2, h));
	}
	rows.add(R1, -1);
// B1 = sum kb Yb
	rows.add(0, 'E', "B1_k");
	for (size_t b(0); b < _input->nB(); ++b) {
		if (partition.label(_input->nR() + b) == label) {
			rows.add(_y[_input->nR() + b], _input->k(_input->nR() + b));
		}
	}
	rows.add(B1, -1);
// B1 = sum 2^l al
	rows.add(0, 'E', "B1_POW");
	for (size_t l(0); l < tB + 1; ++l) {
		rows.add(b[l], std::pow(2, l));
	}
	rows.add(B1, -1);
// Srb <= Yr (r,b) in E
	for (Edge const & edge : _input->edges()) {
		if (partition.label(_input->nR() + edge._j) == label
				&& partition.label(edge._i) == label) {
			// Srb <= Yr (r,b) in E
			rows.add(0, 'L',
					GetStr("S_R", edge._i, "_B", edge._j, "_YR_", edge._i));
			rows.add(_y[edge._i], -1);
			rows.add((int) s.get(edge._i, edge._j), 1);
			// Srb <= Yb (r,b) in E
			rows.add(0, 'L',
					GetStr("S_R", edge._i, "_B", edge._j, "_YB_", edge._j));
			rows.add(_y[_input->nR() + edge._j], -1);
			rows.add((int) s.get(edge._i, edge._j), 1);
		}
	}
// abhl >= ah+bl-1
	for (size_t h(0); h < tR + 1; ++h) {
		for (size_t l(0); l < tB + 1; ++l) {
			rows.add(-1, 'G', GetStr("FORTET_", h, "_", l));
			rows.add((int) ab.get(h, l), 1);
			rows.add(a[h], -1);
			rows.add(b[l], -1);
		}
	}
	rows.add(_env, _lp);
	// maximise
	CPXchgobjsen(_env, _lp, -1);
//	write();
	return cst_obj;
}

// on initialise avec les singletons
void Divisor::write(std::string const & fileName) const {
	CPXwriteprob(_env, _lp, fileName.c_str(), "LP");
}
bool Divisor::run(ModularityBPartition & p, size_t label) {
//MY_PRINT(label);
//MY_PRINT(p.observations(label).size());
	Double const old_obj(p.score(label));
//MY_PRINT(old_obj);
	bool success(false);
	Double const cst_obj(buildLp(p, label));
//	write();
	CPXmipopt(_env, _lp);
	Double new_obj;
	CPXgetobjval(_env, _lp, &new_obj);
	new_obj += cst_obj;
//MY_PRINT(new_obj);
	success = (new_obj > 1e-6 + old_obj);
//	success = true;
	if (success) {
		IntList const temp(p.observations(label));
		size_t const new_label((int) p.getUnUsedLabel());
		std::vector<double> solution(temp.size());
		CPXgetx(_env, _lp, solution.data(), 0, (int) temp.size() - 1);
		// MY_PRINT(temp.size());
		// MY_PRINT(_y.size());
		for (auto const & node : temp) {
			//		std::cout << std::setw(6) << node;
			//		std::cout << std::setw(6) << solution[_y[node]];
			//		std::cout << std::endl;
			if (solution[_y[node]] < 0.5) {
				p.shift(node, new_label);
			}
		}
		p.score() = p.computeScore();
		//MY_PRINT(new_label);
	}

//	MY_PRINT(old_obj);
//	MY_PRINT(cst_obj);
//	MY_PRINT(new_obj);
//	MY_PRINT(p.computeScore());
//	MY_PRINT(_input->m());
	return success;
}
