/*
 * CpxOracle.cpp
 *
 *  Created on: 24 mars 2014
 *      Author: manuel
 */

#include "CpxOracle.h"
#include "ClusteringProblem.h"

CpxOracle::CpxOracle(ClusteringProblem const * input) :
		IOracle(input), _index() {
	_solver = NULL;
}

CpxOracle::~CpxOracle() {
	freeLp();
}

void CpxOracle::initCpx() {
	_solver->initLp("CpxOracle");
	initOracle();

}
void CpxOracle::applyBranchingRule() {
}

void CpxOracle::write(std::string const & fileName) const {
}
bool CpxOracle::generate() {
return false;
}

void CpxOracle::freeLp() {
}

void CpxOracle::checkSolutions() const {
}

