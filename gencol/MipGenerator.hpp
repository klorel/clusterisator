#ifndef MIP_GENERATOR_HPP
#define MIP_GENERATOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"

typedef struct cpxlp*  CPXLPptr;
typedef struct cpxenv* CPXENVptr;

class MipGenerator{
public:
	MipGenerator(BipartiteGraph const &, DoubleVector const & dual);
	~MipGenerator();
public:
	void freeLp();
	void initOracle();
	void initOracle2();
	void writeOracle(std::string const & fileName = "oracle.lp" )const;


	void setUpOracle();

	bool generate();
	std::set<Column > const & columns()const;
	Double bestReducedCost()const;
private:
	CPXENVptr _env;
	CPXLPptr _oracle;
	
	BipartiteGraph const & _input;
	DoubleVector const & _dual;

	std::set<Column > _columns;

	std::vector<int> _index;

	Double _bestReducedCost;
};

#endif 
