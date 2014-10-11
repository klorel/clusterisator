#ifndef QP_GENERATOR_HPP
#define QP_GENERATOR_HPP

#include "gencol.h"
#include "CpxOracle.h"

class QpOracle: public CpxOracle {
public:
	QpOracle(ICliquePartitionProblem const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~QpOracle();
public:
	virtual bool generate();
public:
	virtual void setUpOracle();
	void initOracle();
	void checkSolution() const;
private:
	Double _diagRegularisation;
};

#endif 
