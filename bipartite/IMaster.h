/*
 * IMaster.h
 *
 *  Created on: 23 mars 2014
 *      Author: manuel
 */

#ifndef IMASTER_H_
#define IMASTER_H_

#include "gencol.h"
class BipartiteGraph;

class ModularityBPartition;
class IMaster {
public:
	virtual Double obj() const = 0;

	virtual void solveMaster() = 0;
	virtual bool getSolution(FractionnarySolution &) = 0;
public:
	virtual void add(Column const & column) = 0;
	virtual void add(std::set<Column> const & columns) = 0;
	virtual void add(std::set<Column> const & columns, size_t & nb,
			Double&rd) = 0;
	virtual void add(ReducedCostSorter const & columns, size_t, size_t & nb,
			Double&rd) = 0;
	virtual void add(ModularityBPartition const & column) = 0;
	virtual void add(ModularityBPartition const * column) = 0;
	virtual void addSingleton() = 0;
	virtual void addEdge() = 0;
public:
	virtual void applyBranchingRule() = 0;
	virtual void branchingWeights(FractionnarySolution const &,
			BranchingWeights &) = 0;
public:
//	virtual void buildDualBounds(ModularityBPartition const &) = 0;
//	virtual void buildStabilization(ModularityBPartition const &) = 0;
//	virtual void resetStabilization() = 0;
//	virtual bool stabilized() const = 0;
//
//	virtual bool updateStabilization() = 0;
//	virtual bool centerStabilization() = 0;
//	virtual size_t stabilizationUpdates() const = 0;
//
//	virtual void stabilizationStat(std::ostream & = std::cout) const = 0;
public:

	virtual std::vector<double> const & primal() const = 0;
	virtual std::vector<double> const & dual() const = 0;
	virtual std::set<Column> const & columns() const = 0;
public:
	IMaster(BipartiteGraph const *, DecisionList const * decisions);
	virtual ~IMaster();
protected:
	BipartiteGraph const * _input;
	DecisionList const * _decisions;

	std::set<Column> _columns;
	std::vector<double> _dual;
	std::vector<double> _primal;
};

#endif /* IMASTER_H_ */
