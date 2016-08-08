/*
 * CplexSolver.h
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#ifndef COLUMNSGENERATION_CPLEXSOLVER_H_
#define COLUMNSGENERATION_CPLEXSOLVER_H_

#include "ILpSolver.h"
//#include <cplex.h>

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;

class CplexSolver: public ILpSolver {
public:
	CplexSolver();
	virtual ~CplexSolver();

	virtual int add(RowBuffer & );
	virtual int add(ColumnBuffer & );

	virtual void write(std::string const & fileName) const;

	virtual void initLp(std::string const & name);
	virtual void freeLp();

	virtual void chgObj(IntVector const & indexe, DoubleVector const & values);

	virtual size_t numMipStarts();
	virtual int delMipStarts();

	virtual bool isOptimal()const;
	virtual double objValue()const;

	virtual void applyBranchingRule();
	virtual bool generate();
	virtual void checkSolutions() const;
	virtual void setUpOracle()=0;
	virtual void initOracle()=0;

public:
	virtual void run() {}
	virtual char binary()const { return 'B'; }
	virtual char continuous()const { return 'C'; }

	virtual char leq()const { return 'L'; }
	virtual char eq()const { return 'E'; }
	virtual char geq()const { return 'G'; }
protected:
		CPXENVptr _env;
		CPXLPptr _prob;
};

#endif /* COLUMNSGENERATION_CPLEXSOLVER_H_ */
