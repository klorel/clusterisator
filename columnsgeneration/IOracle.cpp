/*
 * IGenerator.cpp
 *
 *  Created on: 22 mars 2014
 *      Author: manuel
 */

#include "IOracle.h"
#include "ClusteringProblem.h"

IOracle::IOracle(ClusteringProblem const * input) :
		_dual(NULL), _decisions(NULL), _bestReducedCost(0), _input(input) {

}

IOracle::~IOracle() {

}

bool IOracle::generate() {
	return false;
}
ColumnSet const & IOracle::columns() const {
	return _columns;
}
ColumnSet & IOracle::columns() {
	return _columns;
}
Double IOracle::bestReducedCost() const {
	return _bestReducedCost;
}
void IOracle::applyBranchingRule() {

}

void IOracle::sortedColumns(
		std::multimap<Double, Column const *, std::greater<Double>> & result) const {
	result.clear();
//	MY_PRINT(_columns.size());
	for (auto const & column : _columns) {
//		column.print();
		ASSERT_CHECK(column.check(*_dual));
		result.insert(std::make_pair(column.reducedCost(), &column));
	}
}
bool IOracle::run(size_t iteMax, bool stopAtFirst) {
	return false;
}

void IOracle::extract(DoubleVector const & x, Column & column) {
	for (int v(0); v < _input->nV(); ++v) {
		if (x[v] > 0.5) {
			column.insert(v);
		}
	}
	column.cost() = column.computeCost();
}
void IOracle::extractAndAddSolution(DoubleVector const & x, Double rd) {
	Column column(_input);
	extract(x, column);
	column.reducedCost() = rd;
	ASSERT_CHECK(column.check(*_dual));ASSERT_CHECK(column.violation(*_decisions) == 0);
	_columns.insert(column);

}

void IOracle::extractAndAddSolution(DoubleVector const & x) {
	Column column(_input);
	extract(x, column);
	column.reducedCost() = column.computeReducedCost(*_dual);
	assert(column.violation(*_decisions) == 0);
	if (column.reducedCost() > ZERO_REDUCED_COST
			&& column.violation(*_decisions) == 0)
		_columns.insert(column);

}

void IOracle::setData(DoubleVector const & dual,
		DecisionList const & decisions) {
	_dual = &dual;
	_decisions = &decisions;

}
