/*
 * IGenerator.h
 *
 *  Created on: 22 mars 2014
 *      Author: manuel
 */

#ifndef I_ORACLE_H_
#define I_ORACLE_H_

#include "gencol.h"
#include "Decision.hpp"
#include "BipartiteGraph.hpp"
#include "Column.hpp"

class IOracle {
public:
	IOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~IOracle();
public:
	std::set<Column> const & columns() const;
public:
	virtual bool generate();
	virtual Double bestReducedCost() const;
	virtual void applyBranchingRule();

public:
	virtual void sortedColumns(
			std::multimap<Double, Column const *, std::greater<Double>> &) const;
	virtual bool run(size_t iteMax, bool stopAtFirst);
protected:
	BipartiteGraph const * _input;
	DoubleVector const * _dual;
	DecisionList const * _decisions;
	std::set<Column> _columns;
	Double _bestReducedCost;
};

#endif /* IGENERATOR_H_ */
