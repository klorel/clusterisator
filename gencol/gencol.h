#ifndef GENCOL_H
#define GENCOL_H

#include "common.h"

class Column;

typedef std::map<Column const *, Double> FractionnarySolution;

typedef std::multimap<Double, std::pair<size_t, size_t>, std::greater<Double> > BranchingWeights;
typedef std::map<std::pair<size_t, size_t>, Double> BranchingWeights2;

Double const ZERO_REDUCED_COST = 1e-6;
#endif /* CG_MASTER_HPP */
