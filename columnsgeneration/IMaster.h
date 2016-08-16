/*
 * IMaster.h
 *
 *  Created on: 23 mars 2014
 *      Author: manuel
 */

#ifndef I_MASTER_H_
#define I_MASTER_H_

#include "Column.h"
#include "gencol.h"

class CliquePartitionProblem;
class IPartition;
class ColumnBuffer;
class IMaster {
 public:
  virtual Double obj() const = 0;

  virtual void solveMaster() = 0;
  virtual bool getSolution(FractionnarySolution &) = 0;
 public:

  virtual void add(IPartition const & solution) = 0;

  virtual void add(Column const & column) = 0;

  virtual void add(ColumnSet const & columns, int & nb, Double&rd) = 0;
  virtual void add(ReducedCostSorter const & columns, int, int & nb,
                   Double&rd) = 0;

  virtual void addSingleton() = 0;

  virtual void write(std::string const & = "master.lp") const = 0;
 public:
  virtual void applyBranchingRule() = 0;
//	virtual void branchingWeights(FractionnarySolution const &,
//			BranchingWeights &) = 0;
 public:
//	virtual void buildDualBounds(ModularityBPartition const &) = 0;
//	virtual void buildStabilization(ModularityBPartition const &) = 0;
//	virtual void resetStabilization() = 0;
//	virtual bool stabilized() const = 0;
//
//	virtual bool updateStabilization() = 0;
//	virtual bool centerStabilization() = 0;
//	virtual int stabilizationUpdates() const = 0;
//
//	virtual void stabilizationStat(std::ostream & = std::cout) const = 0;
 public:

  virtual std::vector<double> const & primal() const = 0;
  virtual std::vector<double> const & dual() const = 0;
  virtual ColumnSet const & columns() const = 0;
 public:
  IMaster(ClusteringProblem const *, DecisionList const * decisions);
  virtual ~IMaster();
  virtual void getBasis(ColumnBuffer &) const;
  std::string const & log()const { return _log; }
 protected:
  ClusteringProblem const * _input;
  DecisionList const * _decisions;

  ColumnSet _columns;
  std::vector<double> _dual;
  std::vector<double> _primal;
  std::vector<int> _cstat;
  std::vector<int> _rstat;

  std::string _log;
};

#endif /* IMASTER_H_ */
