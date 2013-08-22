#ifndef LP_MASTER_HPP
#define LP_MASTER_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
#include "Decision.hpp"
#include "ModularityBPartition.hpp"
class Node;
typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;

class LpMaster {
public:
	LpMaster(BipartiteGraph const &, DecisionList const & decisions);
	LpMaster(BipartiteGraph const *, DecisionList const * decisions);
	~LpMaster();
public:
	void freeLp();
	void initLp();

	void add(Column const & column);

	void add(ModularityBPartition const & column);
	void add(ModularityBPartition const * column);
	void addSingleton();
	void addEdge();

	void write(std::string const & fileName = "master.lp") const;

	void readColumns(std::string const & fileName);

	void getSolution();
	bool getSolution(FractionnarySolution &);

	void solveMaster();
	void writeColumns(std::string const & = "columns.txt") const;
	Double obj() const;
	std::vector<double> const & primal() const;
	std::vector<double> const & dual() const;
	std::set<Column> const & columns() const;

	void columnsCost(std::vector<double> &);
	void applyBranchingRule();
	void applyBranchingRule(std::vector<double> &);
	void branchingWeights(FractionnarySolution const &, BranchingWeights &);

	void build();

	void checkCost(DecisionList const &) const;
private:
	BipartiteGraph const * _input;
	CPXENVptr _env;
	CPXLPptr _lp;

	std::set<Column> _columns;
	std::vector<double> _dual;
	std::vector<double> _primal;
	std::vector<int> _index;

	Double _obj;
	DecisionList const * _decisions;
//	std::vector<std::vector<std::list<Column const *> > > _rAndbInColumn;
//	std::vector<std::vector<std::list<Column const *> > > _rOrbInColumn;

};

#endif 
