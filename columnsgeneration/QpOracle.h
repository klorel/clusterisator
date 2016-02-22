#ifndef QP_GENERATOR_HPP
#define QP_GENERATOR_HPP

#include "gencol.h"
#include "CpxOracle.h"
#include "CliquePartitionProblem.h"

class QpOracle: public CpxOracle {
public:
	QpOracle(CliquePartitionProblem const *);
	virtual ~QpOracle();
public:
	virtual bool generate();
public:
	virtual void setUpOracle();
	void initOracle();
	void checkSolution() const;
private:
	Double _diagRegularisation;
	CliquePartitionProblem const  * const _cpp;
};

#endif 
