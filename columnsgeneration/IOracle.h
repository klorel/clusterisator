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
#include "Column.hpp"

class IOracle {
public:
	IOracle(ICliquePartitionProblem const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~IOracle();
public:
	Columns & columns();
	Columns const & columns() const;
public:
	virtual bool generate();
	virtual Double bestReducedCost() const;
	virtual void applyBranchingRule();

public:
	virtual void sortedColumns(
			std::multimap<Double, Column const *, std::greater<Double>> &) const;
	virtual bool run(size_t iteMax, bool stopAtFirst);
	virtual void extractAndAddSolution(DoubleVector const & x, Double rd);
	virtual void extractAndAddSolution(DoubleVector const & x);

	virtual void extract(DoubleVector const & x, Column & column);
protected:
	DoubleVector const * _dual;
	DecisionList const * _decisions;
	Columns _columns;
	Double _bestReducedCost;
//private:
	ICliquePartitionProblem const * _input;

};

#endif /* IGENERATOR_H_ */
