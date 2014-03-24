/*
 * CpxOracle.h
 *
 *  Created on: 24 mars 2014
 *      Author: manuel
 */

#ifndef CPXORACLE_H_
#define CPXORACLE_H_

#include "IOracle.h"

#include "LpBuffer.hpp"
#include <cplex.h>
typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class CpxOracle: public IOracle {
public:
	CpxOracle(BipartiteGraph const * input, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~CpxOracle();
public:
	virtual void applyBranchingRule();
	virtual bool generate();
	virtual void setUpOracle()=0;
public:
	void write(std::string const & fileName = "oracle.lp") const;
	void freeLp();
protected:
	CPXENVptr _env;
	CPXLPptr _prob;
	std::vector<int> _index;
	std::vector<std::string> _cname;

	RowBuffer _rowBuffer;
	RowBuffer _decisionBuffer;
};

#endif /* CPXORACLE_H_ */
