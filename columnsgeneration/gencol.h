#ifndef GENCOL_H
#define GENCOL_H

#include "common.h"

class Column;
class Decision;

typedef std::list<Decision> DecisionList;
typedef std::set<Decision> DecisionSet;

typedef std::set<Column> ColumnSet;

typedef std::multimap<Double, Column const *, std::greater<Double> > ReducedCostSorter;
typedef std::map<Column const *, Double> FractionnarySolution;

typedef std::multimap<Double, std::pair<int, int>, std::greater<Double> > BranchingWeights;
typedef std::map<std::pair<int, int>, Double> BranchingWeights2;

Double const ZERO_REDUCED_COST = 1e-6;

//#define ASSERT_CHECK(x) assert(x)
#define ASSERT_CHECK(x)

enum AvailableOracle {
  MILP,
  MIQP
};
#endif /* CG_MASTER_HPP */
