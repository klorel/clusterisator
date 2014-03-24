#ifndef QP_GENERATOR_HPP
#define QP_GENERATOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
#include "CpxOracle.h"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class Node;
class QpOracle: public CpxOracle {
public:
	QpOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~QpOracle();
public:
	virtual bool generate();
public:
	void initOracle();

	virtual void setUpOracle();
	void checkSolution() const;
	void initCpx();
private:
	Double _diagRegularisation;
};

#endif 
