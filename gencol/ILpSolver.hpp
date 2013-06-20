#ifndef I_LP_SOLVER_HPP
#define I_LP_SOLVER_HPP

#include "src/common.h"

class ILpSolver{
public:
	virtual void build(size_t) = 0;
	virtual void addColumns(std::vector<Double> const & obj, std::vector<size_t> const & start, std::vector<size_t> const & size, std::vector<size_t> const & rows);
	
};

#endif /* CG_MASTER_HPP */
