#ifndef MILP_GENERATOR_HPP
#define MILP_GENERATOR_HPP

#include "CliquePartitionProblem.h"
#include "gencol.h"
#include "CpxOracle.h"
struct Product {
	size_t _first;
	size_t _second;
	size_t _firstTimeSecond;
	double _coeff;
};
typedef std::vector<Product> Products;
class MilpOracle: public CpxOracle {
public:
	MilpOracle(CliquePartitionProblem const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~MilpOracle();
public:
	void initOracle();

	virtual void setUpOracle();
	void checkMipSolution() const;
private:
	Products _products;
};

#endif 
