#ifndef LP_MASTER_HPP
#define LP_MASTER_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
class Node;
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
	bool getSolution(FractionnarySolution & );

	void solveMaster();
	void writeColumns(std::string const & = "columns.txt")const;
	Double obj()const;
	DoubleVector const & primal()const;
	DoubleVector const & dual()const;
	std::set<Column > const & columns()const;	
	
	void columnsCost(DoubleVector &);
	void applyBranchingRule(Node const &);
	void applyBranchingRule(Node const &, DoubleVector &);
	void branchingWeights(FractionnarySolution const &, BranchingWeights &);
private:
	CPXENVptr _env;
	CPXLPptr _lp;

	std::set<Column > _columns;
	std::vector<double> _dual;
	std::vector<double> _primal;
	std::vector<int> _index;

	Double _obj;
	
	std::vector< std::vector<std::list<Column const *> > > _rAndbInColumn;
	std::vector< std::vector<std::list<Column const *> > > _rOrbInColumn;

};

#endif 
