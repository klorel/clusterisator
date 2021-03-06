#ifndef UNIPARTITE_BINARY_DECOMPOSITION_ORACLE_HPP
#define UNIPARTITE_BINARY_DECOMPOSITION_ORACLE_HPP

#include "../clustering/RectMatrix.h"
#include "gencol.h"
#include "CpxOracle.h"
#include "UnipartieInstance.h"

class UnipartiteBinaryDecompositionOracle : public CpxOracle {
 public:
  UnipartiteBinaryDecompositionOracle(UnipartieInstance const *);
  virtual ~UnipartiteBinaryDecompositionOracle();
 public:
  void initOracle();

  virtual void setUpOracle();
  virtual void checkSolutions() const;

  void fill(RowBuffer & rowBuffer, ColumnBuffer & columnBuffer);
 private:

  IntVector _s;
  IntVector _a;
  int _tD;
  int _c;
//	RectMatrix _aa;
  IntVector _aa;
  UnipartieInstance const *_uniPartiteGraph;
};

#endif 
