#ifndef MIP_GENERATOR_HPP
#define MIP_GENERATOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class Node;
class MipGenerator {
public:
	MipGenerator(BipartiteGraph const &, DoubleVector const & dual,
			DecisionList const & decisions);
	MipGenerator(BipartiteGraph const *, DoubleVector const * dual,
			DecisionList const * decisions);
	~MipGenerator();
public:
	void freeLp();
	void initOracle();
	void write(std::string const & fileName = "oracle.lp") const;

	void setUpOracle();

	bool generate();
	std::set<Column> const & columns() const;
	Double bestReducedCost() const;
	void initCpx();
	void applyBranchingRule();
	void applyBranchingRule(std::vector<int> &, std::vector<double> &,
			std::vector<double> &);
private:
	CPXENVptr _env;
	CPXLPptr _oracle;

	BipartiteGraph const * _input;
	DoubleVector const * _dual;

	std::set<Column> _columns;

	std::vector<int> _index;

	Double _bestReducedCost;
	RectMatrix _s;
	std::vector<std::string> _cname;
	std::map<std::pair<size_t, size_t>, size_t> _index2;
	std::vector<int> _index3;
	DecisionList const * _decisions;
};

#endif 
