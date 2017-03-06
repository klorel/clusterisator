#include "LpMaster.h"

#include "../clustering/IPartition.h"
#include "../clustering/Timer.h"

#include "../mip_solver/LpBuffer.h"
#include "Node.h"
#include "ClusteringProblem.h"

Double const StabilizationBaseCost = 1;
Double const StabilizationWidth = 1e-4;

LpMaster::LpMaster(ClusteringProblem const *input, DecisionList const * decisions) :
		IMaster(input, decisions) {
	build();
}
Double LpMaster::stabilizationPenalty() {
	return _stabilisator.get_penalty(_solver);
}
void LpMaster::udpate_stabilization() {
	DoubleVector dual(_minus_dual);
	for (auto & v : dual)
		v *= -1;
	_stabilisator.update_pi(_solver, dual);
}
void LpMaster::build() {
	_solver.initLp("LpMaster");
	_solver.setNbThreads(1);
//	_solver.setLog();

//	int err(-1);
//	_env = CPXopenCPLEX(&err);
//
////		CPXsetintparam(_env, CPXPARAM_ScreenOutput, CPX_OFF);
////		CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY, 2);
//	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);
////	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_BARRIER);
//  	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_DUAL);
////  	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_PRIMAL);
//  //	CPXsetintparam(_env, CPX_PARAM_LPMETHOD, CPX_ALG_BAROPT);
//  	CPXsetintparam(_env, CPX_PARAM_PREIND, CPX_ON);
	initLp();
	//	_rAndbInColumn.resize(_input->nR(),
	//			std::vector<std::list<Column const *> >(_input->nB()));
	//	_rOrbInColumn.resize(_input->nR(),
	//			std::vector<std::list<Column const *> >(_input->nB()));

}
LpMaster::~LpMaster() {
	_solver.freeLp();
}

// on initialise avec les singletons
void LpMaster::initLp() {
	RowBuffer rowBuffer;
	for (int v(0); v < _input->nV(); ++v)
		rowBuffer.add(1, 'E', GetStr("ROW_Y", v));
	_minus_dual.assign(rowBuffer.size(), 1);
	_rstat.assign(rowBuffer.size(), 0);
	_solver.add(rowBuffer);
	_solver.maximize();

//	_stabilisator.set(_minus_dual, 2, .1);
//	ColumnBuffer stabBuffer(_solver.continuous());
//	_stabilisator.build(_solver, stabBuffer);
//	_solver.add(stabBuffer);
//
//	_minus_dual = {
//		-0.0164120691831839,
//		-0.0279005176114127,
//		-0.00820603459159197,
//		-0.00858477464966545,
//		-0.0041661406388083,
//		-0.00845852796364082,
//		-0.00656482767327358,
//		-0.00214619366241637,
//		-0.0061860876152001,
//		-0.00530236081302865,
//		-0.00871102133568992,
//		-0.0253755838909228,
//		-0.0171695492993309,
//		-0.0277742709253882,
//		-0.00871102133568994,
//		-0.00517611412700417,
//		-0.00719606110339608,
//		-0.00214619366241633,
//		0,
//		0,
//		-0.024618103774776,
//		-0.0190632495896982,
//		-0.022345663426335,
//		-0.0100997348819594,
//		-0.00290367377856325,
//		-0.0100997348819594,
//		-0.0214619366241636,
//		-0.00681732104532261,
//		-0.0111097083701553,
//		-0.0208307031940412,
//		0,
//		0
//	};
//	_stabilisator.update_pi(_solver, _minus_dual);
//	std::exit(0);
	//	int i(0);
	//	for (auto const & column : _columns) {
	//		column.id() = i;
	//		_index.push_back((int) i);
	//		for (auto const & r : column.r()) {
	//			for (int b(0); b < _input->nB(); ++b)
	//				_rOrbInColumn[r][b].push_back(&column);
	//		}
	//		for (int r(0); r < _input->nR(); ++r) {
	//			for (auto const & b : column.b()) {
	//				_rOrbInColumn[r][b].push_back(&column);
	//			}
	//		}
	//		++i;
	//	}
}
void LpMaster::write(std::string const & fileName) const {
	_solver.write(fileName);
}

void LpMaster::add(ReducedCostSorter const & columns, int nmax, int & nb, Double&rd) {
	ColumnBuffer columnBuffer(_solver.continuous());
	rd = -1;
	nb = 0;
	int const current_n(_solver.ncols());
	for (auto const & kvp : columns) {
		if (nmax > 0 && nb >= nmax)
			break;
		Column const & column(*kvp.second);
		add(column, columnBuffer, current_n, rd, nb);
	}
	if (columnBuffer.size() > 0)
		_solver.add(columnBuffer);
	_primal.resize(_solver.ncols());
}

void LpMaster::add(Column const & column, ColumnBuffer & columnBuffer, int current_n, Double & rd, int &nb) {

	ASSERT_CHECK(column.check(_minus_dual));
	auto result(_columns.insert(column));
	if (!result.second) {
		std::cout << "column already here : " << result.first->id() << std::endl;
		MY_PRINT(result.first->reducedCost());
		MY_PRINT(result.first->cost());
		MY_PRINT(result.first->violation(*_decisions));
		double obj;
//		CPXgetobj(_env, _lp, &obj, (int)result.first->id(),
//			(int)result.first->id());
		MY_PRINT(obj);
		assert(column.check(_minus_dual)); 
		assert(column.violation(*_decisions) == 0);
		std::cout << "column is : ";
		for (auto const v : column.v()) {
			std::cout << v <<" ";
		}
		std::cout << std::endl;
		exit(0);
	} else {
		rd = std::max(rd, column.reducedCost());
		result.first->id() = current_n + nb;
		columnBuffer.add(column.cost(), _solver.continuous(), 0, _solver.infinity(), GetStr("COLUMN_", result.first->id()));
		for (auto const & v : column.v()) {
			columnBuffer.add(v, 1);
		}
		++nb;
	}
}
void LpMaster::add(ColumnSet const & columns, int & nb, Double&rd) {
	ColumnBuffer columnBuffer(_solver.continuous());
	int const current_n(_solver.ncols());
	nb = 0;
	rd = -1;
	for (Column const & column : columns) {
		add(column, columnBuffer, current_n, rd, nb);
	}
	if (columnBuffer.size() > 0)
		_solver.add(columnBuffer);
	_primal.resize(_primal.size() + columnBuffer.size());
	assert(nb == columnBuffer.size());
}

void LpMaster::add(ColumnSet const & columns) {
	int nb;
	Double rd;
	add(columns, nb, rd);
}

void LpMaster::add(Column const & column) {
	int const current_n(_solver.ncols());
	ColumnBuffer columnBuffer(_solver.continuous());
	Double rd;
	int nb(0);
	add(column, columnBuffer, current_n, rd, nb);

	if (columnBuffer.size() > 0)
		_solver.add(columnBuffer);

	_primal.resize(_solver.ncols());
}
void LpMaster::addSingleton() {
	for (int v(0); v < _input->nV(); ++v) {
		Column column(_input);
		column.insert(v);
		column.cost() = 0;
		column.reducedCost() = 0;
		add(column);
	}
}

void LpMaster::readColumns(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	std::string line;
	int i;
	int old((int) _columns.size());
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
	std::cout << "Read " << _columns.size() - old << " columns from file " << fileName << std::endl;
}

bool PrimalPredicate::operator()(DoubleVector const & v1, DoubleVector const & v2) const {
	//return v1 < v2;
	for (int i(0); i < v1.size(); ++i) {
		if (v1[i] - v2[i] < -1e-10)
			return true;
		else if (v1[i] - v2[i] > 1e-10)
			return false;
	}
	return false;

}
bool PrimalPredicate::operator()(Int2Double const & v1, Int2Double const & v2) const {
	Int2Double::const_iterator it1(v1.begin());
	Int2Double::const_iterator end1(v1.end());
	Int2Double::const_iterator it2(v2.begin());
	Int2Double::const_iterator end2(v2.end());

	while (it1 != end1 && it2 != end2) {
		if (it1->first == it2->first) {
			// same index, compare value
			if (it1->second - it2->second < -1e-10)
				return true;
			else if (it1->second - it2->second > 1e-10)
				return false;
		} else if (it1->first < it2->first) {
			if (it1->second < -1e-10)
				return true;
			else if (it1->second > 1e-10)
				return false;

		} else if (it1->first > it2->first) {
			// same index, compare value
			if (-it2->second < -1e-10)
				return true;
			else if (-it2->second > 1e-10)
				return false;
		}
		++it1;
		++it2;
	}
	if (it1 == end1 && it2 == end2) {
		return false;
	} else if (it1 != end1 && it2 == end2) {
		if (-it2->second < -1e-10)
			return true;
		else if (-it2->second > 1e-10)
			return false;
	} else if (it1 == end1 && it2 != end2) {
		if (it1->second < -1e-10)
			return true;
		else if (it1->second > 1e-10)
			return false;
	}
	return false;
}

void LpMaster::getSolution() {
	assert(_solver.nrows() == _minus_dual.size());
	_solver.dual(_minus_dual);
	for (int i(0); i < _minus_dual.size(); ++i) {
		//		std::cout << i << " : " << _dual[i] << std::endl;
		_minus_dual[i] *= -1;
		//if(std::fabs(_dual[i])<1e-10)
		//	_dual[i] = 0;
	}
	assert(_solver.ncols() == _primal.size());
	_solver.primal(_primal);
	_obj = _solver.objValue();
//	_cstat.resize(_solver.ncols());
//	CPXgetbase(_env, _lp, _cstat.data(), _rstat.data());
//	int nBasis(0);
//	int nBasisDegen(0);
//	IntSet basis;
//	Int2Double sol;

//	for (int i(0); i < _cstat.size(); ++i) {
	//if (_cstat[i] == CPX_BASIC)
	//	std::cout << "STATUS[" << i << "] = CPX_BASIC" << std::endl;
	//else if (_cstat[i] == CPX_AT_LOWER)
	//	std::cout << "STATUS[" << i << "] = CPX_AT_LOWER" << std::endl;
	//else if (_cstat[i] == CPX_AT_UPPER)
	//	std::cout << "STATUS[" << i << "] = CPX_AT_UPPER" << std::endl;
	//else if (_cstat[i] == CPX_FREE_SUPER)
	//	std::cout << "STATUS[" << i << "] = CPX_FREE_SUPER" << std::endl;
//		if (_cstat[i] == CPX_BASIC) {
//			basis.insert(i);
//			++nBasis;
//		}
//		if (_cstat[i] == CPX_BASIC && _primal[i] < 1e-6)
//			++nBasisDegen;
//		if (std::fabs(_primal[i]) > 1e-10) {
//			sol[i] = _primal[i];
//		}
//	}
	//for (int i(0); i < _primal.size(); ++i) {
	//if(_primal[i]>1e-10)
	//std::cout << "x[" << i << "] = "<<std::setprecision(10) << _primal[i] << ", ";
	//}
	//std::cout << std::endl<<" basis ("<<basis.size()<<") = ";
	//for (auto const b : basis)
	//	std::cout << b << ", ";
	//std::cout << std::endl;
//	_allBasis[sol].insert(basis);
	//std::cout << std::setw(4)<<_allBasis[_primal].size() << "\tNumber of basis related to primal solution "<< std::endl;
	//_log = GetStr(_allBasis[sol].size());
	_log = "";
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
	//	write();
	_solver.run();
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

ColumnSet const & LpMaster::columns() const {
	return _columns;
}

std::vector<double> const & LpMaster::dual() const {
	return _minus_dual;
}

DoubleVector const & LpMaster::primal() const {
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
	_solver.chgObj(index, obj);
//	CPXchgobj(_env, _lp, (int)index.size(), index.data(), obj.data());
}

void LpMaster::add(IPartition const & solution) {
	ColumnSet columns;
	for (auto const & label : solution.usedLabels()) {
		Column c(_input);
		for (auto const & n : solution.observations(label)) {
			c.insert(n);
		}
		c.cost() = c.computeCost();
		c.reducedCost() = c.computeReducedCost(_minus_dual);
		//add(c);
		if (_columns.find(c) == _columns.end())
			columns.insert(c);
//		for (auto const & n : solution.observations(label)) {
//			Column clone(c);
//			clone.erase(n - 1);
//			clone.cost() = clone.computeCost();
//			clone.reducedCost() = clone.computeReducedCost(_dual);
//			columns.insert(clone);
//		}
	}
	add(columns);
}
