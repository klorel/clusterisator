/*
 * Stabilisators.h
 *
 *  Created on: 17 sept. 2016
 *      Author: manuel
 */

#ifndef COLUMNSGENERATION_STABILISATORS_H_
#define COLUMNSGENERATION_STABILISATORS_H_

#include "gencol.h"

#include "../mip_solver/LpBuffer.h"
#include "../mip_solver/ILpSolver.h"
#include "../mip_solver/CplexSolver.h"

class Stabilisator {
public:
	Stabilisator();
	Stabilisator(DoubleVector const & pi, Double penalty, Double width);
	void set(DoubleVector const & pi, Double penalty, Double width);
public:

	DoubleVector &pi_bar();
	DoubleVector const &pi_bar() const;

	Double delta1(int i) const;
	Double delta2(int i) const;
	Double gamma1(int i) const;
	Double gamma2(int i) const;

	Double dseta_m() const;
	Double epsilon_m() const;
	Double epsilon_p() const;
	Double dseta_p() const;

	void build(CplexSolver & solver, ColumnBuffer & buffer);
	void update_pi(CplexSolver & solver, DoubleVector const & pi);
	Double get_penalty(CplexSolver & solver);


public:
	DoubleVector _pi_bar;

	Double _penalty;
	Double _width;

	IntVector _indexes;
	DoubleVector _cost;
	DoubleVector _dual_cost;
	DoubleVector _x;
	DoubleVector _rc;
};

#endif /* COLUMNSGENERATION_STABILISATORS_H_ */
