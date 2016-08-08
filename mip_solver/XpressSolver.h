/*
 * XpressSolver.h
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#ifndef COLUMNSGENERATION_XPRESSSOLVER_H_
#define COLUMNSGENERATION_XPRESSSOLVER_H_

#include "ILpSolver.h"

typedef struct xo_prob_struct* XPRSprob;
class XpressSolver: public ILpSolver {
public:
	XpressSolver();
	virtual ~XpressSolver();

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
public:

	virtual void run();

	virtual char binary()const { return 'B'; }
	virtual char continuous()const { return 'C'; }

	virtual char leq()const { return 'L'; }
	virtual char eq()const { return 'E'; }
	virtual char geq()const { return 'G'; }



public:
	XPRSprob _lp;
	void errormsg(const char *sSubName, int nLineno, int nErrorCode);
	bool _is_mip;

	static bool WasInit;
};

#endif /* COLUMNSGENERATION_XPRESSSOLVER_H_ */
