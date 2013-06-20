#ifndef I_LP_SOLVER_HPP
#define I_LP_SOLVER_HPP

#include "gencol.h"
/*
* specific lp interface done for column generation algorithm
* master initialisation
* slave initialisationS
*/
class ILpSolver{
public:
	virtual void addColumns(
		std::vector<double> const & obj, 
		std::vector<size_t> const & start,
		std::vector<size_t> const & size,
		std::vector<size_t> const & rows
	)=0;
	virtual void setObj(std::vector<double> const & ) = 0;
	virtual void addObj(std::vector<size_t> const &, std::vector<double> const & ) = 0;

protected:
	Graph const & _input;	
};

// l'idée c'est de faire des wrapper à partir de ILpSolver

class MasterSolver{
public:
	virtual void init() = 0;
	virtual void init(Columns const & ) = 0;
	virtual void add(Columns const & ) = 0;
protected:
	ILpSolver & _solver;
};

class SlaveSolver{
public:
	virtual void init() = 0;
	// Ryan and Foster rule : (i,j) must or cannot be together
	virtual void addConstraint(size_t , size_t, bool ) =0;
	virtual void setDualValues(double const * const lambda)=0;
	virtual bool run(Columns & );
protected:
	ILpSolver & _solver;
	std::vector<double> _initCost;
};


#endif /* CG_MASTER_HPP */
