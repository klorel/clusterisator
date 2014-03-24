/*
 * IGenerator.cpp
 *
 *  Created on: 22 mars 2014
 *      Author: manuel
 */

#include "IOracle.h"

IOracle::IOracle(BipartiteGraph const * input, DoubleVector const * dual,
		DecisionList const * decisions) :
		_input(input), _dual(dual), _decisions(decisions), _bestReducedCost(0) {

}

IOracle::~IOracle() {

}

bool IOracle::generate() {
	return false;
}
std::set<Column> const & IOracle::columns() const {
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
	for (auto const & column : _columns) {
		result.insert(std::make_pair(column.reducedCost(), &column));
	}
}
bool IOracle::run(size_t iteMax, bool stopAtFirst) {
	return false;
}
void IOracle::extractAndAddSolution(DoubleVector const & x, Double rd) {
	Column column(_input);
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
	column.reducedCost() = rd;
	column.check(*_dual);

	for (Decision const & decision : *_decisions) {
		if (column.violation(decision) > 0) {
			decision.print(
					std::cout << "violation in MipGenerator::generate() ");
			std::cout << std::endl;
			column.print();
		}
	}
	assert(column.violation(*_decisions) == 0);
	_columns.insert(column);

}
