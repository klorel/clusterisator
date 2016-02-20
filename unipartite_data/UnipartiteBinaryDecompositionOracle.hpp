#ifndef UNIPARTITE_BINARY_DECOMPOSITION_ORACLE_HPP
#define UNIPARTITE_BINARY_DECOMPOSITION_ORACLE_HPP

#include "gencol.h"
#include "CpxOracle.h"
#include "UnipartieInstance.h"
#include "RectMatrix.hpp"

class UnipartiteBinaryDecompositionOracle: public CpxOracle {
public:
	UnipartiteBinaryDecompositionOracle(UnipartieInstance const *,
			DoubleVector const * dual, DecisionList const * decisions);
	virtual ~UnipartiteBinaryDecompositionOracle();
public:
	void initOracle();

	virtual void setUpOracle();
	virtual void checkSolutions() const;
private:

	IntVector _s;
	IntVector _a;
	size_t _tD;
	size_t _c;
//	RectMatrix _aa;
	IntVector _aa;
	UnipartieInstance const *_uniPartiteGraph;
};

#endif 
