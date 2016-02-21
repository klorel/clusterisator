#ifndef BINARY_DECOMPOSITION_ORACLE_HPP
#define BINARY_DECOMPOSITION_ORACLE_HPP

#include "BipartiteGraph.h"
#include "gencol.h"
#include "CpxOracle.h"

class BinaryDecompositionOracle: public CpxOracle {
public:
	BinaryDecompositionOracle(BipartiteGraph const *);
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
