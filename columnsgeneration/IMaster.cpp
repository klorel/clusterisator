/*
 * IMaster.cpp
 *
 *  Created on: 23 mars 2014
 *      Author: manuel
 */

#include "IMaster.h"

#include "Column.h"
#include "Decision.h"
#include "LpBuffer.h"

#include <cplex.h>
#include "ClusteringProblem.h"

IMaster::IMaster(ClusteringProblem const * input,
		DecisionList const * decisions) :
		_input(input), _decisions(decisions), _columns(), _dual(), _primal(), _cstat(), _rstat() {

}

IMaster::~IMaster() {

}

void IMaster::getBasis(ColumnBuffer & result) const {
	result.clear();
	for (auto const & column : _columns) {
		size_t const id(column.id());
		if (_cstat[id] == CPX_BASIC) {
			result.add(0, CPX_CONTINUOUS, 0, CPX_INFBOUND, GetStr("C_", id));
			for (auto const & v : column.v()) {
				result.add(v, 1);
			}
		}
	}
}
