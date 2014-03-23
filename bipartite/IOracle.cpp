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
