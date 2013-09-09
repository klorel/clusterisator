#include "LpMaster.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"
#include "LpBuffer.hpp"
#include <cplex.h>

Double const StabilizationBaseCost = 1;
Double const StabilizationWidth = 1e-4;

LpMaster::LpMaster(BipartiteGraph const &input, DecisionList const & decisions) :
		_input(&input), _env(NULL), _lp(NULL), _decisions(&decisions) {
	build();
	_stabilizationCost = StabilizationBaseCost;
}
LpMaster::LpMaster(BipartiteGraph const *input, DecisionList const * decisions) :
		_input(input), _env(NULL), _lp(NULL), _decisions(decisions) {
	build();
	_stabilizationCost = StabilizationBaseCost;
}

void LpMaster::resetStabilization() {
	if (_stabilized) {
//		_stabilizationCost = StabilizationBaseCost;
//		_stabilizationUpdates = 0;

	}
}
bool LpMaster::stabilized() const {
	return _stabilized;
}
void LpMaster::build() {
	int err;
	_env = CPXopenCPLEX(&err);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
	//CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_BARRIER);
//	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_DUAL);
	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_PRIMAL);
//	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_BAROPT);
//	CPXsetintparam(_env, CPX_PARAM_PREIND, CPX_OFF);
	initLp();
	//	_rAndbInColumn.resize(_input->nR(),
	//			std::vector<std::list<Column const *> >(_input->nB()));
	//	_rOrbInColumn.resize(_input->nR(),
	//			std::vector<std::list<Column const *> >(_input->nB()));
	_stabilized = false;
}
LpMaster::~LpMaster() {
	freeLp();
}
void LpMaster::freeLp() {
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
	if (_lp != NULL) {
		CPXfreeprob(_env, &_lp);
		_lp = NULL;
	}
}

// on initialise avec les singletons
void LpMaster::initLp() {
	if (_lp != NULL) {
		CPXfreeprob(_env, &_lp);
		_lp = NULL;
	}
	int err(0);
	_lp = CPXcreateprob(_env, &err, "LpMaster");
	RowBuffer rowBuffer;
	for (size_t v(0); v < _input->nV(); ++v)
		rowBuffer.add(1, 'E', GetStr("ROW_Y", v));
	_dual.resize(rowBuffer.size());
	rowBuffer.add(_env, _lp);
	assert(CPXgetnumrows(_env, _lp) == rowBuffer.size());
	CPXchgobjsen(_env, _lp, -1);
	//	size_t i(0);
	//	for (auto const & column : _columns) {
	//		column.id() = i;
	//		_index.push_back((int) i);
	//		for (auto const & r : column.r()) {
	//			for (size_t b(0); b < _input->nB(); ++b)
	//				_rOrbInColumn[r][b].push_back(&column);
	//		}
	//		for (size_t r(0); r < _input->nR(); ++r) {
	//			for (auto const & b : column.b()) {
	//				_rOrbInColumn[r][b].push_back(&column);
	//			}
	//		}
	//		++i;
	//	}
}
void LpMaster::write(std::string const & fileName) const {
	CPXwriteprob(_env, _lp, fileName.c_str(), "LP");
}

void LpMaster::add(ReducedCostSorter const & columns, size_t nmax, size_t & nb,
		Double&rd) {
	ColumnBuffer columnBuffer;
	rd = -1;
	nb = 0;
	size_t const current_n(CPXgetnumcols(_env, _lp));
	for (auto const & kvp : columns) {
		if (nmax > 0 && nb >= nmax)
			break;
		Column const & column(*kvp.second);
		auto result(_columns.insert(column));
		if (!result.second) {
			//		std::cout << "column already here : " << result.first->id()
			//				<< std::endl;
			//		result.first->print();
			//		double obj;
			//		CPXgetobj(_env, _lp, &obj, (int) result.first->id(),
			//				(int) result.first->id());
			//		std::cout << "column cost is " << obj << std::endl;
			//		std::cout << "column violation is "
			//				<< result.first->violation(*_decisions) << std::endl;
			//		exit(0);
		} else {
			rd = std::max(rd, column.reducedCost());
			result.first->id() = current_n + nb;
			columnBuffer.add(column.cost(), CPX_CONTINUOUS, 0, CPX_INFBOUND,
					GetStr("COLUMN_", result.first->id()));
			for (auto const & r : column.r()) {
				columnBuffer.add(r, 1);
			}
			for (auto const & b : column.b()) {
				columnBuffer.add(_input->nR() + b, 1);
			}
			++nb;
		}
	}
	if (columnBuffer.size() > 0)
		columnBuffer.add(_env, _lp);
	_primal.resize(CPXgetnumcols(_env, _lp));
}
void LpMaster::add(std::set<Column> const & columns, size_t & nb, Double&rd) {
	ColumnBuffer columnBuffer;
	size_t const current_n(CPXgetnumcols(_env, _lp));
	nb = 0;
	rd = -1;
	for (Column const & column : columns) {
		auto result(_columns.insert(column));
		if (!result.second) {
			//		std::cout << "column already here : " << result.first->id()
			//				<< std::endl;
			//		result.first->print();
			//		double obj;
			//		CPXgetobj(_env, _lp, &obj, (int) result.first->id(),
			//				(int) result.first->id());
			//		std::cout << "column cost is " << obj << std::endl;
			//		std::cout << "column violation is "
			//				<< result.first->violation(*_decisions) << std::endl;
			//		exit(0);
		} else {
			rd = std::max(rd, column.reducedCost());
			result.first->id() = current_n + nb;
			columnBuffer.add(column.cost(), CPX_CONTINUOUS, 0, CPX_INFBOUND,
					GetStr("COLUMN_", result.first->id()));
			for (auto const & r : column.r()) {
				columnBuffer.add(r, 1);
			}
			for (auto const & b : column.b()) {
				columnBuffer.add(_input->nR() + b, 1);
			}
			++nb;
		}
	}
	if (columnBuffer.size() > 0)
		columnBuffer.add(_env, _lp);
	_primal.resize(_primal.size() + columnBuffer.size());
	assert(nb == columnBuffer.size());
}

void LpMaster::add(std::set<Column> const & columns) {
	size_t nb;
	Double rd;
	add(columns, nb, rd);
}

void LpMaster::add(Column const & column) {
	//writeColumns("bug.txt");
	auto result(_columns.insert(column));
	if (!result.second) {
		//		std::cout << "column already here : " << result.first->id()
		//				<< std::endl;
		//		result.first->print();
		//		double obj;
		//		CPXgetobj(_env, _lp, &obj, (int) result.first->id(),
		//				(int) result.first->id());
		//		std::cout << "column cost is " << obj << std::endl;
		//		std::cout << "column violation is "
		//				<< result.first->violation(*_decisions) << std::endl;
		//		exit(0);
	} else {
		result.first->id() = CPXgetnumcols(_env, _lp);
		ColumnBuffer columnBuffer;
		columnBuffer.add(column.cost(), CPX_CONTINUOUS, 0, CPX_INFBOUND,
				GetStr("COLUMN_", result.first->id()));
		for (auto const & r : column.r()) {
			columnBuffer.add(r, 1);
		}
		for (auto const & b : column.b()) {
			columnBuffer.add(_input->nR() + b, 1);
		}
		columnBuffer.add(_env, _lp);

		//		CPXaddcols(_env, _lp, (int) ccnt, (int) nzcnt, obj.data(),
		//				cmatbeg.data(), rmatind.data(), rmatval.data(), lb.data(),
		//				ub.data(), NULL);

		_primal.resize(CPXgetnumcols(_env, _lp));
		//		std::vector<bool> _rIn(_input->nR(), false);
		//		for (auto const & r : result.first->r())
		//			_rIn[r].flip();
		//
		//		std::vector<bool> _bIn(_input->nB(), false);
		//		for (auto const & b : result.first->b())
		//			_bIn[b].flip();

		//	for (size_t r(0); r < _input->nR(); ++r) {
		//		for (size_t b(0); b < _input->nB(); ++b) {
		//			if (_rIn[r] && _bIn[b])
		//				_rAndbInColumn[r][b].push_back(&*result.first);
		//			else if (!_rIn[r] && _bIn[b])
		//				_rOrbInColumn[r][b].push_back(&*result.first);
		//			else if (_rIn[r] && !_bIn[b])
		//				_rOrbInColumn[r][b].push_back(&*result.first);
		//		}
		//	}
	}
}
void LpMaster::addSingleton() {
	DoubleVector emptyDual(_input->nV(), 0);
	for (size_t r(0); r < _input->nR(); ++r) {
		Column column(_input);
		column.addElement(1 + r);
		column.cost() = 0;
		column.reducedCost() = column.cost();
		add(column);
	}
	for (size_t b(0); b < _input->nB(); ++b) {
		Column column(_input);
		column.addElement(1 + _input->nR() + b);
		column.cost() = 0;
		column.reducedCost() = column.cost();
		add(column);
	}
}
void LpMaster::addEdge() {
	for (auto const & e : _input->edges()) {
		Column column(_input);
		column.addElement(1 + e._i);
		column.addElement(1 + _input->nR() + e._j);
		column.cost() = _input->w(e._i, e._j);
		column.reducedCost() = column.cost();
		add(column);
	}
}

void LpMaster::readColumns(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	std::string line;
	size_t i;
	size_t old(_columns.size());
	while (std::getline(file, line)) {
		if (!line.empty()) {
			Column column(_input);
			std::stringstream buffer(line);
			while (buffer >> i) {
				column.addElement(i);
			}
			column.cost() = column.computeCost();
			column.reducedCost() = column.cost();
			add(column);
		}
	}
	file.close();
	std::cout << "Read " << _columns.size() - old << " columns from file "
			<< fileName << std::endl;
}

void LpMaster::getSolution() {
	assert(CPXgetnumrows(_env, _lp) == _dual.size());
	CPXgetpi(_env, _lp, _dual.data(), 0, (int) (_dual.size() - 1));
	for (size_t i(0); i < _dual.size(); ++i) {
		_dual[i] *= -1;
		//if(std::fabs(_dual[i])<1e-10)
		//	_dual[i] = 0;
	}
	assert(CPXgetnumcols(_env, _lp) == _primal.size());
	CPXgetx(_env, _lp, _primal.data(), 0, (int) (_primal.size() - 1));
	CPXgetobjval(_env, _lp, &_obj);
}

bool LpMaster::getSolution(FractionnarySolution & solution) {
	solution.clear();
	bool is_integer(true);
	for (auto const & column : _columns) {
		if (std::fabs(_primal[column.id()]) > 1e-6) {
			solution[&column] = _primal[column.id()];
			if (std::fabs(_primal[column.id()]) < 1 - 1e-6) {
				is_integer = false;
				//				std::cout << "not integeral " << _primal[column.id()]
				//						<< std::endl;
			}
		}
	}
	//	MY_PRINT(is_integer);
	return is_integer;
}

void LpMaster::solveMaster() {
	//write();
	CPXlpopt(_env, _lp);
	getSolution();
}

void LpMaster::writeColumns(std::string const & fileName) const {
	std::ofstream file(fileName.c_str());
	for (auto const & column : _columns) {
		for (auto const & r : column.r())
			file << std::setw(6) << r + 1;
		for (auto const & b : column.b())
			file << std::setw(6) << _input->nR() + b + 1;
		file << std::endl;
	}
	file.close();
}

std::set<Column> const & LpMaster::columns() const {
	return _columns;
}

std::vector<double> const & LpMaster::dual() const {
	return _dual;
}

std::vector<double> const & LpMaster::primal() const {
	return _primal;
}

Double LpMaster::obj() const {
	return _obj;
}

void LpMaster::applyBranchingRule() {
	std::vector<double> obj;
	std::vector<int> index;
	index.reserve(_columns.size());
	obj.reserve(index.size());
	for (auto const & column : _columns) {
		index.push_back(column.id());
		if (column.violation(*_decisions) > 0)
			obj.push_back(-1e6);
		else
			obj.push_back(column.cost());
		//		assert(std::fabs(column.cost() - column.computeCost()) < 1e-6);
	}
	CPXchgobj(_env, _lp, (int) index.size(), index.data(), obj.data());
}
void LpMaster::branchingWeights(FractionnarySolution const & solution,
		BranchingWeights & weights) {
	// on cherche des arrêtes présentes et semi-présentes dans deux colonnes
	BranchingWeights2 temp;
	std::map<Edge, std::pair<IntSet, IntSet> > toto;
	for (auto const & kvp : solution) {
		//		std::cout << std::setw(6) << kvp.first->id();
		//		std::cout << std::setw(15) << kvp.second;
		//		std::cout << std::endl;
		for (Edge const & e : _input->edges()) {
			bool const iR(kvp.first->constainsR(e._i));
			bool const iB(kvp.first->constainsB(e._j));
			if (iR && iB) {
				toto[e].first.insert(kvp.first->id());
			} else if (iR + iB == 1) {
				toto[e].second.insert(kvp.first->id());
			}
		}

	}
	//	std::cout << "synthese" << std::endl;
	weights.clear();
	for (auto const & t : toto) {
		//		std::cout << std::setw(6) << t.second.first.size();
		//		std::cout << std::setw(6) << t.second.second.size();
		//		std::cout << std::endl;
		if (!t.second.first.empty() && !t.second.second.empty()) {
			//			std::cout << std::setw(6) << t.first._i;
			//			std::cout << std::setw(6) << t.first._j;
			//			std::cout << std::endl;
			weights.insert(
					std::make_pair(
							0.5
									* ((int) ((t.second.first.size()
											+ t.second.second.size()))),
							std::make_pair(t.first._i, t.first._j)));
		}
	}
	if (weights.empty()) {
		std::cout << "weights.empty(), generating full weights" << std::endl;
		for (auto const & kvp : solution) {
			for (size_t r(0); r < _input->nR(); ++r) {
				for (size_t b(0); b < _input->nB(); ++b) {
					bool const iR(kvp.first->constainsR(r));
					bool const iB(kvp.first->constainsB(b));
					if (iR && iB) {
						toto[Edge(r, b, 1)].first.insert(kvp.first->id());
					} else if (iR + iB == 1) {
						toto[Edge(r, b, 1)].second.insert(kvp.first->id());
					}
				}
			}
		}
		std::cout << "weights.empty()" << std::endl;

		for (auto const & t : toto) {
			//		std::cout << std::setw(6) << t.second.first.size();
			//		std::cout << std::setw(6) << t.second.second.size();
			//		std::cout << std::endl;
			if (!t.second.first.empty() && !t.second.second.empty()) {
				//			std::cout << std::setw(6) << t.first._i;
				//			std::cout << std::setw(6) << t.first._j;
				//			std::cout << std::endl;
				weights.insert(
						std::make_pair(
								0.5
										* ((int) ((t.second.first.size()
												+ t.second.second.size()))),
								std::make_pair(t.first._i, t.first._j)));
			}
		}
	}
}

void LpMaster::add(ModularityBPartition const & solution) {
	std::set<Column> columns;
	for (size_t const & label : solution.usedLabels()) {
		Column c(_input);
		for (size_t const & n : solution.observations(label)) {
			c.addElement(n + 1);
		}
		c.cost() = c.computeCost();
		//add(c);
		columns.insert(c);
		for (size_t const & n : solution.observations(label)) {
			Column clone(c);
			clone.delElement(n);
			clone.cost() = clone.computeCost();
			columns.insert(clone);
		}
		//for (size_t const & n1 : solution.observations(label)) {
		//	for (size_t const & n2 : solution.observations(label)) {
		//		if(n1<n2){
		//			Column clone(c);
		//			clone.delElement(n1);
		//			clone.delElement(n2);
		//			clone.cost() = clone.computeCost();
		//			columns.insert(clone);
		//		}		
		//	}
		//}
	}
	add(columns);
	//write();
}
void LpMaster::add(ModularityBPartition const * solution) {
	if (solution != NULL) {
		add(*solution);
	}
}

void LpMaster::buildDualBounds(ModularityBPartition const & partition) {
	_dualLower.resize(_input->nV());
	_dualUpper.assign(_input->nV(), -1e20);
	for (size_t v(0); v < _input->nV(); ++v) {
		_dualLower[v] = partition.score(partition.label(v))
				- partition.scoreIfSwap(partition.label(v), v);
		for (size_t const & otherLabel : partition.usedLabels()) {
			if (otherLabel != partition.label(v)) {
				Double const candidate(
						partition.scoreIfSwap(otherLabel, v)
								- partition.score(otherLabel));
				if (_dualUpper[v] < candidate) {
					_dualUpper[v] = candidate;
				}
			}
		}
		if (_dualLower[v] > _dualUpper[v])
			std::swap(_dualLower[v], _dualUpper[v]);
//		MY_PRINT(std::fabs(_dualLower[v] - _dualUpper[v]));
		if (std::fabs(_dualLower[v] - _dualUpper[v]) < StabilizationWidth) {
			_dualLower[v] -= StabilizationWidth / 2;
			_dualUpper[v] += StabilizationWidth / 2;
		}

//		Double midle(0.5 * (_dualLower[v] + _dualUpper[v]));
//		Double half_range(0.5 * std::abs(_dualLower[v] - _dualUpper[v]));
//		Double const coeff(0.005);
//		_dualLower[v] = midle - half_range;
//		_dualUpper[v] = midle + half_range;
//		_dualLower[v] = midle - coeff;
//		_dualUpper[v] = midle + coeff;
		//		_dualLower[v] -= 0.0001;
		//		_dualUpper[v] += 0.0001;
	}
}

void LpMaster::buildStabilization(ModularityBPartition const &partition) {
	_stabilized = true;
	buildDualBounds(partition);
	resetStabilization();

	_epsIndex.clear();
	_yIndex.clear();
	_yCost.clear();
	_epsIndex.reserve(2 * _input->nV());
	_yIndex.reserve(2 * _input->nV());
	_yCost.reserve(2 * _input->nV());

	size_t const current_nrows(CPXgetnumrows(_env, _lp));
	size_t const current_ncols(CPXgetnumcols(_env, _lp));

	RowBuffer rows;
	ColumnBuffer columns;

	for (size_t v(0); v < _input->nV(); ++v) {
		_epsIndex.push_back((int) (current_nrows + rows.size()));
		rows.add(_stabilizationCost, 'L', GetStr("EPS_Y_MOINS_", v));
		_epsIndex.push_back((int) (current_nrows + rows.size()));
		rows.add(_stabilizationCost, 'L', GetStr("EPS_Y_PLUS_", v));
	}

	for (size_t v(0); v < _input->nV(); ++v) {
		_yIndex.push_back((int) (current_ncols + columns.size()));
		_yCost.push_back(_dualLower[v]);
		columns.add(_dualLower[v], CPX_CONTINUOUS, 0, CPX_INFBOUND,
				GetStr("STAB_Y_MOINS_", v));
		// ax - y- 
		columns.add(v, -1);
		// y- <= stab_cost
		columns.add(current_nrows + columns.size() - 1, +1);

		_yIndex.push_back((int) (current_ncols + columns.size()));
		_yCost.push_back(_dualUpper[v]);
		columns.add(-_dualUpper[v], CPX_CONTINUOUS, 0, CPX_INFBOUND,
				GetStr("STAB_Y_PLUS_", v));
		// ax + y+
		columns.add(v, +1);
		// y+ <= stab_cost
		columns.add(current_nrows + columns.size() - 1, +1);

		//		std::cout << std::setw(20) << _dualLower[v];
		//		std::cout << std::setw(20) << _dualUpper[v];
		//		std::cout << std::endl;
	}
	//write();
	rows.add(_env, _lp);
	//write();
	columns.add(_env, _lp);
	//write();
	_dual.resize(CPXgetnumrows(_env, _lp));
	_primal.resize(CPXgetnumcols(_env, _lp));

}
bool LpMaster::centerStabilization() {
	bool point_moved(false);
	if (_stabilized) {
		for (size_t v(0); v < _input->nV(); ++v) {
			//_dual
			Double const dual(_dual[v]);
			if (dual < _dualLower[v] - 1e-6 || dual > _dualUpper[v] + 1e-6) {
//				if (dual < _dualLower[v] + 1e-6 || dual > _dualUpper[v] - 1e-6) {
				Double width((_dualUpper[v] - _dualLower[v]) / 2);
				width = std::max(width, 1e-4);
				_dualLower[v] = dual - width;
				_yCost[2 * v] = _dualLower[v];
				_dualUpper[v] = dual + width;
				_yCost[2 * v + 1] = _dualUpper[v];
				point_moved = true;
				resetStabilization();
//				MY_PRINT(width);
			}
		}
	}
	return point_moved;
}

bool LpMaster::updateStabilization() {
	bool launch_exact(false);
	if (_stabilized) {
		// on ajuste les bornes si on est dehors
		bool adjust_penalty(!centerStabilization());
		// on diminue la pénalité
		if (adjust_penalty) {
			++_stabilizationUpdates;
			_stabilizationCost = StabilizationBaseCost
					/ std::pow(2, 2 * _stabilizationUpdates);
//			_stabilizationCost *= _stabilizationCost;

			if (_stabilizationCost < 1e-6)
				_stabilizationCost = 0;
			std::vector<double> temp(_epsIndex.size(), _stabilizationCost);
			CPXchgrhs(_env, _lp, (int) _epsIndex.size(), _epsIndex.data(),
					temp.data());
			if (_stabilizationCost > 1e-10) {
				std::cout << "STABILISATION adjust penalty "
						<< std::setprecision(15) << _stabilizationCost
						<< std::endl;
			} else {
				std::cout << "STABILISATION launch exact"
						<< std::setprecision(15) << std::endl;
				launch_exact = true;
			}
		} else {
			resetStabilization();
			std::vector<double> temp(_epsIndex.size(), _stabilizationCost);
			CPXchgrhs(_env, _lp, (int) _epsIndex.size(), _epsIndex.data(),
					temp.data());
			std::cout << "STABILISATION shift point" << std::endl;
			CPXchgobj(_env, _lp, (int) _yIndex.size(), _yIndex.data(),
					_yCost.data());
		}
	}
	return launch_exact;
}

void LpMaster::stabilizationStat(std::ostream & stream) const {
	if (stabilized()) {
		size_t outOfBound(0);
		size_t inBound(0);

		for (size_t v(0); v < _input->nV(); ++v) {
			Double const dual(_dual[v]);
			if (dual < _dualLower[v] - 1e-6 || dual > _dualUpper[v] + 1e-6) {
				++outOfBound;
			} else if (std::fabs(dual - _dualLower[v]) <= 1e-6
					|| std::fabs(dual - _dualUpper[v]) <= 1e-6) {
				++inBound;
			}
		}
		stream << std::setw(4) << outOfBound;
		stream << std::setw(4) << inBound;
		stream << std::setw(4) << _input->nV() - inBound - outOfBound;
		stream << std::setw(20) << _stabilizationCost;
		stream << std::endl;

//		stream << "outOfBound            : " << outOfBound << std::endl;
//		stream << "inBound               : " << inBound << std::endl;
//		stream << "other                 : "
//				<< _input->nV() - inBound - outOfBound << std::endl;
//		stream << "_stabilizationCost    : " << _stabilizationCost << std::endl;
//		stream << "StabilizationBaseCost : " << StabilizationBaseCost
//				<< std::endl;

	}
}

size_t LpMaster::stabilizationUpdates() const {
	return _stabilizationUpdates;
}
