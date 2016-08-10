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
	virtual void add(RowBuffer &);
	virtual void add(ColumnBuffer &);

	virtual void write(std::string const & fileName) const;

	virtual void initLp(std::string const & name);
	virtual void freeLp();

	virtual void chgObj(IntVector const & indexe, DoubleVector const & values);

	virtual void  delMipStarts();

	virtual bool isOptimal()const;
	virtual double objValue()const;

	virtual char binary()const;
	virtual char continuous()const;
	virtual char leq()const;
	virtual char eq()const;
	virtual char geq()const;

	virtual void run();
public:
	bool & is_minimize();
	bool is_minimize()const;

	void minimize();
	void maximize();

	void add(std::ostream &);

	std::list<std::ostream *> & stream();

protected:
	bool _is_minimize;
	bool _is_mip;
	std::list<std::ostream *> _stream;
};

#endif /* COLUMNSGENERATION_ILPSOLVER_H_ */
