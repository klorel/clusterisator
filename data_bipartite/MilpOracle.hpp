#ifndef MILP_GENERATOR_HPP
#define MILP_GENERATOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"

#include "CpxOracle.h"
class MilpOracle: public CpxOracle {
public:
	MilpOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~MilpOracle();
public:
	void initOracle();

	virtual void setUpOracle();
	void checkMipSolution() const;
private:
	RectMatrix _s;
	BipartiteGraph const *_biPartiteGraph;
};

#endif 
