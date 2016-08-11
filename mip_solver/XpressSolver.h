/*
 * XpressSolver.h
 *
 *  Created on: 8 ao�t 2016
 *      Author: manuruiz
 */

#ifndef COLUMNSGENERATION_XPRESSSOLVER_H_
#define COLUMNSGENERATION_XPRESSSOLVER_H_

#include "ILpSolver.h"

//#define __LAZY_XPRESS__
typedef struct xo_prob_struct* XPRSprob;
class XpressSolver : public ILpSolver {
 public:
#ifndef __LAZY_XPRESS__
  virtual void add(RowBuffer &);
  virtual void add(ColumnBuffer &);

  virtual void write(std::string const & fileName) const;

  virtual void initLp(std::string const & name);
  virtual void freeLp();

  virtual void chgObj(IntVector const & indexe, DoubleVector const & values);

  virtual void delMipStarts();

  virtual bool isOptimal() const;
  virtual double objValue() const;
  virtual void objValue(int i, Double & obj) const;
  virtual void solution(int i, DoubleVector & result) const;
  virtual int ncols()const;
  virtual int nrows()const;
 public:

  virtual void run();

  virtual void setNbThreads(int);

  virtual void setLog();
  virtual void setNoLog();

  virtual double infinity() const;

  virtual char binary() const;
  virtual char continuous() const;

  virtual char leq() const;
  virtual char eq() const;
  virtual char geq() const;
#endif
 public:
  XpressSolver();
  virtual ~XpressSolver();
  void errormsg(const char *sSubName, int nLineno, int nErrorCode);
 public:
  XPRSprob _lp;

  static bool WasInit;
};

#endif /* COLUMNSGENERATION_XPRESSSOLVER_H_ */
