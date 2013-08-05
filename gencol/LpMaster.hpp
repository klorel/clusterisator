#ifndef LP_MASTER_HPP
#define LP_MASTER_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"

typedef struct cpxlp*  CPXLPptr;
typedef struct cpxenv* CPXENVptr;

class LpMaster{
public:
	LpMaster(BipartiteGraph const &);
	~LpMaster();
public:
	BipartiteGraph const & _input;
	void freeLp();
	void initLp();
	void write(std::string const & fileName = "master.lp" )const;

	void add(Column const & column);
	void readColumns(std::string const & fileName);
	

	void getSolution();

	void solveMaster();
	void writeColumns(std::string const & = "columns.txt")const;
	Double obj()const;
	DoubleVector const & primal()const;
	DoubleVector const & dual()const;
	std::set<Column > const & columns()const;	

private:
	CPXENVptr _env;
	CPXLPptr _lp;

	std::set<Column > _columns;
	std::vector<double> _dual;
	std::vector<double> _primal;

	Double _obj;


};

#endif 
