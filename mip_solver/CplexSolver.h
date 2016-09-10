/*
 * CplexSolver.h
 *
 *  Created on: 8 ao�t 2016
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
	using ILpSolver::add;
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
  virtual void delRows(int first, int last);
  virtual bool isOptimal() const;
  virtual double objValue() const;
  virtual void objValue(int i, Double & obj) const;
  virtual void solution(int i, DoubleVector & result) const;


  virtual int ncols()const;
  virtual int nrows()const;


  virtual void applyBranchingRule(DecisionList const & decision, RowBuffer & rowBuffer, RowBuffer & decisionBuffer);
  virtual bool generate();
  virtual void checkSolutions() const;
  virtual void setUpOracle();
  virtual void initOracle();

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
 protected:
  CPXENVptr _env;
  CPXLPptr _prob;
#endif
};

#endif /* COLUMNSGENERATION_CPLEXSOLVER_H_ */
