/*
 * IGenerator.h
 *
 *  Created on: 22 mars 2014
 *      Author: manuel
 */

#ifndef I_ORACLE_H_
#define I_ORACLE_H_

#include "Column.h"
#include "Decision.h"
#include "gencol.h"
class ClusteringProblem;
class CliquePartitionProblem;
class IOracle {
public:
	IOracle(ClusteringProblem const *);
	virtual ~IOracle();
public:
	ColumnSet & columns();
	ColumnSet const & columns() const;
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

	virtual void setData(DoubleVector const & dual,
			DecisionList const & decisions);
protected:
	DoubleVector const * _dual;
	DecisionList const * _decisions;
	ColumnSet _columns;
	Double _bestReducedCost;
//private:
	ClusteringProblem const * _input;

};

#endif /* IGENERATOR_H_ */
