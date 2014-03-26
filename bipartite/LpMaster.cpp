#include "LpMaster.hpp"
#include "BipartiteGraph.hpp"
#include "BinaryDecompositionOracle.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"
#include "LpBuffer.hpp"
#include <cplex.h>

Double const StabilizationBaseCost = 1;
Double const StabilizationWidth = 1e-4;

LpMaster::LpMaster(BipartiteGraph const *input, DecisionList const * decisions) :
		IMaster(input, decisions), _env(NULL), _lp(NULL) {
	build();
}

void LpMaster::build() {
	int err;
	_env = CPXopenCPLEX(&err);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
//	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_ON);
	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
	//CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_BARRIER);
//	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_DUAL);
//	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_PRIMAL);
//	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_BAROPT);
//	CPXsetintparam(_env, CPX_PARAM_PREIND, CPX_OFF);
	initLp();
	//	_rAndbInColumn.resize(_input->nR(),
	//			std::vector<std::list<Column const *> >(_input->nB()));
	//	_rOrbInColumn.resize(_input->nR(),
	//			std::vector<std::list<Column const *> >(_input->nB()));

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
	_dual.assign(rowBuffer.size(), 0);
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
		add(column, columnBuffer, current_n, rd, nb);
	}
	if (columnBuffer.size() > 0)
		columnBuffer.add(_env, _lp);
	_primal.resize(CPXgetnumcols(_env, _lp));
}

void LpMaster::add(Column const & column, ColumnBuffer & columnBuffer,
		size_t current_n, Double & rd, size_t &nb) {

	ASSERT_CHECK(column.check(_dual));
	auto result(_columns.insert(column));
	if (!result.second) {
		std::cout << "column already here : " << result.first->id()
				<< std::endl;
		//		result.first->print();
		MY_PRINT(result.first->reducedCost());
		MY_PRINT(result.first->cost());
		MY_PRINT(result.first->violation(*_decisions));
		double obj;
		CPXgetobj(_env, _lp, &obj, (int) result.first->id(),
				(int) result.first->id());
		MY_PRINT(obj);
		//		exit(0);
		ASSERT_CHECK(column.check(_dual));
		ASSERT_CHECK(column.violation(*_decisions) == 0);
	} else {
		rd = std::max(rd, column.reducedCost());
		result.first->id() = current_n + nb;
		columnBuffer.add(column.cost(), CPX_CONTINUOUS, 0, CPX_INFBOUND,
				GetStr("COLUMN_", result.first->id()));
		for (auto const & v : column.v()) {
			columnBuffer.add(v, 1);
		}
		++nb;
	}
}
void LpMaster::add(std::set<Column> const & columns, size_t & nb, Double&rd) {
	ColumnBuffer columnBuffer;
	size_t const current_n(CPXgetnumcols(_env, _lp));
	nb = 0;
	rd = -1;
	for (Column const & column : columns) {
		add(column, columnBuffer, current_n, rd, nb);
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
	size_t const current_n(CPXgetnumcols(_env, _lp));
	ColumnBuffer columnBuffer;
	Double rd;
	size_t nb(0);
	add(column, columnBuffer, current_n, rd, nb);
//	//writeColumns("bug.txt");
//	auto result(_columns.insert(column));
//	if (!result.second) {
//		//		std::cout << "column already here : " << result.first->id()
//		//				<< std::endl;
//		//		result.first->print();
//		//		double obj;
//		//		CPXgetobj(_env, _lp, &obj, (int) result.first->id(),
//		//				(int) result.first->id());
//		//		std::cout << "column cost is " << obj << std::endl;
//		//		std::cout << "column violation is "
//		//				<< result.first->violation(*_decisions) << std::endl;
//		//		exit(0);
//	} else {
//		result.first->id() = CPXgetnumcols(_env, _lp);
//		ColumnBuffer columnBuffer;
//		columnBuffer.add(column.cost(), CPX_CONTINUOUS, 0, CPX_INFBOUND,
//				GetStr("COLUMN_", result.first->id()));
//		for (auto const & r : column.r()) {
//			columnBuffer.add(r, 1);
//		}
//		for (auto const & b : column.b()) {
//			columnBuffer.add(_input->nR() + b, 1);
//		}
	if (columnBuffer.size() > 0)
		columnBuffer.add(_env, _lp);

	_primal.resize(CPXgetnumcols(_env, _lp));
}
void LpMaster::addSingleton() {
	DoubleVector emptyDual(_input->nV(), 0);
	for (size_t r(0); r < _input->nR(); ++r) {
		Column column(_input);
		column.insert(r);
		column.cost() = 0;
		column.reducedCost() = column.cost();
		add(column);
	}
	for (size_t b(0); b < _input->nB(); ++b) {
		Column column(_input);
		column.insert(_input->nR() + b);
		column.cost() = 0;
		column.reducedCost() = column.cost();
		add(column);
	}
}
void LpMaster::addEdge() {
	for (auto const & e : _input->edges()) {
		Column column(_input);
		column.insert(e._i);
		column.insert(_input->nR() + e._j);
		column.cost() = column.computeCost();
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
				column.insert(i - 1);
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
//		std::cout << i << " : " << _dual[i] << std::endl;
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
		for (auto const & v : column.v())
			file << std::setw(6) << v + 1;
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
		assert(column.check());
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
			bool const iR(kvp.first->contains(e._i));
			bool const iB(kvp.first->contains(e._j + _input->nR()));
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
					bool const iR(kvp.first->contains(r));
					bool const iB(kvp.first->contains(b) + _input->nR());
					if (iR && iB) {
						toto[Edge(r, b, 1)].first.insert(kvp.first->id());
					} else if (iR + iB == 1) {
						toto[Edge(r, b, 1)].second.insert(kvp.first->id());
					}
				}
			}
		}

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
		if (weights.empty())
			std::cout << "weights.empty()" << std::endl;
	}
}

void LpMaster::add(ModularityBPartition const & solution) {
	std::set<Column> columns;
	for (size_t const & label : solution.usedLabels()) {
		Column c(_input);
		for (size_t const & n : solution.observations(label)) {
			c.insert(n);
		}
		c.cost() = c.computeCost();
		c.reducedCost() = c.computeReducedCost(_dual);
		//add(c);
		columns.insert(c);
		for (size_t const & n : solution.observations(label)) {
			Column clone(c);
			clone.erase(n - 1);
			clone.cost() = clone.computeCost();
			clone.reducedCost() = clone.computeReducedCost(_dual);
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

