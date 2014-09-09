#ifndef QP_GENERATOR_HPP
#define QP_GENERATOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "CpxOracle.h"


class BipartiteGraph;
class QpOracle: public CpxOracle {
public:
	QpOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~QpOracle();
public:
	virtual bool generate();
public:

	virtual void setUpOracle();
	void initOracle();
	void checkSolution() const;
private:
	BipartiteGraph const *_biPartiteGraph;
	Double _diagRegularisation;
};

#endif 
