/*
 * IMaster.cpp
 *
 *  Created on: 23 mars 2014
 *      Author: manuel
 */

#include "IMaster.h"

#include "Column.h"
#include "Decision.h"

IMaster::IMaster(CliquePartitionProblem const * input,
		DecisionList const * decisions) :
		_input(input), _decisions(decisions), _columns(), _dual(), _primal() {

}

IMaster::~IMaster() {

}

