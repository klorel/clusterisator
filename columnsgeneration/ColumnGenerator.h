#ifndef COLUMN_GENERATOR_HPP
#define COLUMN_GENERATOR_HPP

#include "gencol.h"
#include "IOracle.h"
class ClusteringProblem;
class ColumnGenerator {
 public:
  ColumnGenerator(ClusteringProblem const *);
  virtual ~ColumnGenerator();
 public:

 public:
  void setExact(IOracle * oracle, DoubleVector const & dual,
                DecisionList const & list);
  void setVns(IOracle * oracle, DoubleVector const & dual,
              DecisionList const & list);

  bool run();

  ReducedCostSorter const & result() const;

  void setNumberByIte(int);
  int getNumberByIte()const;
  Double rc() const;

  Double vnsTime() const;
  Double exactTime() const;

  std::string const & step() const;
  void applyBranchingRule();
  void addNeighbor(int neighborhood_size);
 private:
  bool vns();
  bool exact();
  void clear();


  Double reducedCost(Column const & rhs)const;
 private:
  IOracle * _vns;
  IOracle * _exact;

  Double _exactTime;
  Double _vnsTime;

  std::string _step;
  int _nColumnsByIte;
  Double _rc;

  ReducedCostSorter _result;
  ColumnSet _neighbor;

  ClusteringProblem const * _input;
  DoubleVector const * _minus_dual;
  DecisionList const * _list;

};

#endif 
