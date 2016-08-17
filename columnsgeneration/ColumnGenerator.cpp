#include "ColumnGenerator.h"

#include "BranchAndBound.h"
#include "Timer.h"

ColumnGenerator::ColumnGenerator() :
		_vns(NULL), _exact(NULL), _exactTime(0), _vnsTime(0), _step(""), _nColumnsByIte(
				0), _rc(0) {

}
ColumnGenerator::~ColumnGenerator() {

}

int ColumnGenerator::getNumberByIte()const {
	return _nColumnsByIte;
}

void ColumnGenerator::setExact(IOracle * oracle, DoubleVector const & dual,
		DecisionList const & list) {
	_exact = oracle;
	_exact->setData(dual, list);
}
void ColumnGenerator::setVns(IOracle * oracle, DoubleVector const & dual,
		DecisionList const & list) {
	_vns = oracle;
	_vns->setData(dual, list);

}
void ColumnGenerator::clear() {
	_result.clear();
	_vns->columns().clear();
	_exact->columns().clear();

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
	for (int i(0); i < 10 && !stopvns; ++i) {
		if (_nColumnsByIte == 0) {
			if (_vns->run(1, true)) {
				heuristicSucceeded = true;
				stopvns = true;
			}
		} else {
			if (_vns->run(1, false))
				heuristicSucceeded = true;
			if (_vns->columns().size() > _nColumnsByIte)
				stopvns = true;
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
	//stop = !generate();
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

	return !_result.empty();
}
