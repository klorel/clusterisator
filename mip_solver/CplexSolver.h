/*
 * CplexSolver.h
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#ifndef COLUMNSGENERATION_CPLEXSOLVER_H_
#define COLUMNSGENERATION_CPLEXSOLVER_H_

#include "ILpSolver.h"

#ifndef __LAZY_CPLEX__
typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
#endif

class CplexSolver : public ILpSolver {
public:
	CplexSolver();
	virtual ~CplexSolver();
#ifndef __LAZY_CPLEX__
	virtual void add(RowBuffer &);
	virtual void add(ColumnBuffer &);

	virtual void write(std::string const & fileName) const;

	virtual void initLp(std::string const & name);
	virtual void freeLp();

	virtual void chgObj(IntVector const & indexe, DoubleVector const & values);

	virtual void delMipStarts();

	virtual bool isOptimal()const;
	virtual double objValue()const;

	virtual void applyBranchingRule();
	virtual bool generate();
	virtual void checkSolutions() const;
	virtual void setUpOracle();
	virtual void initOracle();

public:
	virtual void run();
	virtual char binary()const;
	virtual char continuous()const;

	virtual char leq()const;
	virtual char eq()const;
	virtual char geq()const;
protected:
	CPXENVptr _env;
	CPXLPptr _prob;
#endif
};

#endif /* COLUMNSGENERATION_CPLEXSOLVER_H_ */
