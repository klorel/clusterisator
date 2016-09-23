/*
 * IMaster.cpp
 *
 *  Created on: 23 mars 2014
 *      Author: manuel
 */

#include "IMaster.h"

#include "Column.h"
#include "Decision.h"
#include "../mip_solver/LpBuffer.h"

#include "ClusteringProblem.h"
#include <cplex.h>
IMaster::IMaster(ClusteringProblem const * input,
                 DecisionList const * decisions)
    : _input(input),
      _decisions(decisions),
      _columns(),
      _minus_dual(),
      _primal(),
      _cstat(),
      _rstat() {

}

IMaster::~IMaster() {

}

void IMaster::getBasis(ColumnBuffer & result) const {
  result.clear();
  for (auto const & column : _columns) {
    int const id(column.id());
    if (_cstat[id] == CPX_BASIC) {
      result.add(0, CPX_CONTINUOUS, 0, CPX_INFBOUND, GetStr("C_", id));
      for (auto const & v : column.v()) {
        result.add(v, 1);
      }
    }
  }
}
