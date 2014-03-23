#ifndef MILP_GENERATOR_HPP
#define MILP_GENERATOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
#include "LpBuffer.hpp"
#include "IOracle.h"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class Node;
class MilpOracle: public IOracle {
public:
	MilpOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~MilpOracle();
public:
	virtual void applyBranchingRule();
	virtual bool generate();
public:
	void freeLp();
	void initOracle();
	void write(std::string const & fileName = "oracle.lp") const;

	void setUpOracle();

	void initCpx();
	void checkMipSolution()const;
private:
	CPXENVptr _env;
	CPXLPptr _oracle;

	std::vector<int> _index;

	std::vector<std::string> _cname;

	RowBuffer _rowBuffer;
	RowBuffer _decisionBuffer;
	RectMatrix _s;
};

#endif 
