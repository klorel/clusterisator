#ifndef BINARY_DECOMPOSITION_ORACLE_HPP
#define BINARY_DECOMPOSITION_ORACLE_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
#include "LpBuffer.hpp"
#include "IOracle.h"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class Node;
class BinaryDecompositionOracle: public IOracle {
public:
	BinaryDecompositionOracle(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	virtual ~BinaryDecompositionOracle();
public:
	virtual void applyBranchingRule();
	virtual bool generate();
public:
	void freeLp();
	void initOracle();
	void write(std::string const & fileName = "oracle.lp") const;

	void setUpOracle();

	void initCpx();
private:
	CPXENVptr _env;
	CPXLPptr _oracle;

	std::vector<int> _index;

	std::vector<std::string> _cname;

	RowBuffer _rowBuffer;
	RowBuffer _decisionBuffer;
};

#endif 
