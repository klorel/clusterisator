/*
 * ILpSolver.h
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#ifndef COLUMNSGENERATION_ILPSOLVER_H_
#define COLUMNSGENERATION_ILPSOLVER_H_

#include "gencol.h"

class RowBuffer;
class ColumnBuffer;

class ILpSolver {
public:
	ILpSolver();
	virtual ~ILpSolver();
public:
	virtual int add(RowBuffer & )=0;
	virtual int add(ColumnBuffer & )=0;

	virtual void write(std::string const & fileName) const=0;

	virtual void initLp(std::string const & name)=0;
	virtual void freeLp()=0;

	virtual void chgObj(IntVector const & indexe, DoubleVector const & values)=0;

	virtual size_t numMipStarts()=0;
	virtual int delMipStarts()=0;

	virtual bool isOptimal()const=0;
	virtual double objValue()const=0;

	virtual char binary()const = 0;
	virtual char continuous()const = 0;

	virtual char leq()const = 0;
	virtual char eq()const = 0;
	virtual char geq()const = 0;
public:
	virtual void run() = 0;
public:
	bool & is_minimize();
	bool is_minimize()const;

	void minimize();
	void maximize();

	void add(std::ostream &);

	std::list<std::ostream *> & stream();

protected:
	bool _is_minimize;
	std::list<std::ostream *> _stream;
};

#endif /* COLUMNSGENERATION_ILPSOLVER_H_ */
