#ifndef BINARY_DECOMPOSITION_ORACLE_HPP
#define BINARY_DECOMPOSITION_ORACLE_HPP

#include "gencol.h"
#include "CpxOracle.h"
#include "BipartiteGraph.hpp"

class BinaryDecompositionOracle: public CpxOracle {
public:
	BinaryDecompositionOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~BinaryDecompositionOracle();
public:
	void initOracle();

	virtual void setUpOracle();
	virtual void checkSolutions();
private:

	RectMatrix _s;
	RectMatrix _ab;
	BipartiteGraph const *_biPartiteGraph;
};

#endif 