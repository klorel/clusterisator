
#pragma once

#include "common.h"
#include "SdpProblem.h"

class SdpSolver {
public:
	SdpSolver(SdpProblem const &);
	~SdpSolver();
public:
	void launch_mosek();
private:
	SdpProblem const & _input;
public:
	DoubleVector _y;
};