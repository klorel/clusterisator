#include "ColumnGenerator.h"

#include "BranchAndBound.h"
#include "Timer.h"
#include "ClusteringProblem.h"

ColumnGenerator::ColumnGenerator(ClusteringProblem const * input) :
		_vns(NULL), _exact(NULL), _exactTime(0), _vnsTime(0), _step(""), _nColumnsByIte(0), _rc(0), _input(input), _minus_dual(NULL), _list(NULL) {

}
ColumnGenerator::~ColumnGenerator() {

}

int ColumnGenerator::getNumberByIte() const {
	return _nColumnsByIte;
}

void ColumnGenerator::setExact(IOracle * oracle, DoubleVector const & dual, DecisionList const & list) {
	_exact = oracle;
	_exact->setData(dual, list);
	_minus_dual = &dual;
	_list = &list;
}
void ColumnGenerator::setVns(IOracle * oracle, DoubleVector const & dual, DecisionList const & list) {
	_vns = oracle;
	_vns->setData(dual, list);
	_minus_dual = &dual;
	_list = &list;

}
void ColumnGenerator::clear() {
	_result.clear();
	_vns->columns().clear();
	_exact->columns().clear();
}
void ColumnGenerator::addNeighbor() {
	_neighbor.clear();
	ReducedCostSorter temp_result;
	for (auto const & kvp : _result) {
		_neighbor.insert(*kvp.second);
	}
	for (auto const & kvp : _result) {
		if (kvp.first > ZERO_REDUCED_COST) {
			for (auto const i : kvp.second->v()) {
				Column neighbor(_input);
				neighbor.v() = kvp.second->v();
				neighbor.v().erase(i);
				if (_neighbor.find(neighbor) == _neighbor.end()) {
					Double const neighbor_rc(neighbor.computeReducedCost(*_minus_dual));
//					std::cout << std::setw(6)<<i;
//					std::cout << std::setw(15)<<neighbor_rc;
//					std::cout << std::endl;

					if (neighbor.violation(*_list) == 0 && neighbor_rc > ZERO_REDUCED_COST) {
						neighbor.cost() = neighbor.computeCost();
						neighbor.reducedCost() = neighbor_rc;
						auto it = _neighbor.insert(neighbor).first;
						temp_result.insert( { neighbor_rc, &*it });
					}
				}
			}
		}
	}
	_result.insert(temp_result.begin(), temp_result.end());
//	std::cout << "Neighbor generated "<<_neighbor.size()-n<<" new columns"<<std::endl;
}

Double ColumnGenerator::reducedCost(Column const & rhs) const {
	Double result(0);
	result = _input->computeCost(rhs.v());
	return result;
}
ReducedCostSorter const & ColumnGenerator::result() const {
	return _result;
}
void ColumnGenerator::setNumberByIte(int nColumnsByIte) {
	_nColumnsByIte = nColumnsByIte;
}

Double ColumnGenerator::rc() const {
	return _rc;
}

Double ColumnGenerator::vnsTime() const {
	return _vnsTime;
}
Double ColumnGenerator::exactTime() const {
	return _exactTime;
}
std::string const & ColumnGenerator::step() const {
	return _step;
}
void ColumnGenerator::applyBranchingRule() {
	_exact->applyBranchingRule();
	_vns->applyBranchingRule();
}

bool ColumnGenerator::vns() {
	Timer timer;
	bool heuristicSucceeded(false);
	bool stopvns(false);
	for (int i(0); i < 1 && !stopvns; ++i) {
		if (_nColumnsByIte == 0) {
			if (_vns->run(1, false)) {
				heuristicSucceeded = true;
				//stopvns = true;
			}
		}
		else {
			if (_vns->run(1, false))
				heuristicSucceeded = true;
			if (_vns->columns().size() > _nColumnsByIte)
				stopvns = true;
		}
	}
	for (int i(0); i < 5 && !stopvns; ++i) {
		if (_nColumnsByIte == 0) {
			if (_vns->run(1, true)) {
				heuristicSucceeded = true;
				//stopvns = true;
			}
		}
	}

	_vnsTime += timer.elapsed();
	_rc = -1;
	if (heuristicSucceeded) {
		_step = "HEURISTIC";
		timer.restart();
		_vns->sortedColumns(_result);
		_rc = _result.begin()->first;
	}
	return heuristicSucceeded;
}
bool ColumnGenerator::exact() {
	Timer timer;
	_step = "EXACT";
	timer.restart();
	bool stop = !_exact->generate();
	_exactTime += timer.elapsed();
	if (!stop) {
		timer.restart();
		_exact->sortedColumns(_result);
		_rc = _result.begin()->first;
	} else
		_rc = _exact->bestReducedCost();
	return !stop;
}

bool ColumnGenerator::run() {
	clear();
	bool stop(false);
	if (!stop && _vns != NULL && vns()) {
		stop = true;
	}
	if (!stop && _exact != NULL) {
		exact();
	}
	ReducedCostSorter neighbors;

	return !_result.empty();
}
