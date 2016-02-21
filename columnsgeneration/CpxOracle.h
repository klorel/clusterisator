/*
 * CpxOracle.h
 *
 *  Created on: 24 mars 2014
 *      Author: manuel
 */

#ifndef CPXORACLE_H_
#define CPXORACLE_H_

#include "IOracle.h"
#include <cplex.h>
#include "LpBuffer.h"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;

#include "CliquePartitionProblem.h"

class CpxOracle: public IOracle {
public:
	CpxOracle(CliquePartitionProblem const * input);
	virtual ~CpxOracle();
public:
	virtual void applyBranchingRule();
	virtual bool generate();
	virtual void setUpOracle()=0;
	virtual void initOracle()=0;
	virtual void checkSolutions() const;
public:
	void initCpx();
	void write(std::string const & fileName = "oracle.lp") const;
	void freeLp();
protected:
	CPXENVptr _env;
	CPXLPptr _prob;
	std::vector<int> _index;

	RowBuffer _rowBuffer;
	RowBuffer _decisionBuffer;
};

#endif /* CPXORACLE_H_ */
