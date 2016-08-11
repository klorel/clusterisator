/*
 * CpxOracle.h
 *
 *  Created on: 24 mars 2014
 *      Author: manuel
 */

#ifndef CPXORACLE_H_
#define CPXORACLE_H_

#include "IOracle.h"
#include "../mip_solver/LpBuffer.h"
#include "../mip_solver/ILpSolver.h"

class CpxOracle : public IOracle {
 public:
  CpxOracle(ClusteringProblem const * input);
  virtual ~CpxOracle();
 public:
  virtual void applyBranchingRule();
  virtual bool generate();
  virtual void setUpOracle()=0;
  virtual void initOracle()=0;
  virtual void checkSolutions() const;
 public:
  void initCpx();
  void freeLp();
 protected:
  std::vector<int> _index;

  RowBuffer _rowBuffer;
  RowBuffer _decisionBuffer;

  ILpSolverPtr _solver;
};

#endif /* CPXORACLE_H_ */
