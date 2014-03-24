#ifndef BINARY_DECOMPOSITION_ORACLE_HPP
#define BINARY_DECOMPOSITION_ORACLE_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
#include "LpBuffer.hpp"
#include "CpxOracle.h"

class Node;
class BinaryDecompositionOracle: public CpxOracle {
public:
	BinaryDecompositionOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~BinaryDecompositionOracle();
public:
	void initOracle();

	virtual void setUpOracle();

	void initCpx();

	void checkSolutions();
private:

	RectMatrix _s;
	RectMatrix _ab;
};

#endif 
