#ifndef LP_MASTER_HPP
#define LP_MASTER_HPP

#include "gencol.h"
#include "IMaster.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
#include "Decision.hpp"
#include "ModularityBPartition.hpp"
class Node;
typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class ColumnBuffer;
class LpMaster: public IMaster {
public:
	LpMaster(BipartiteGraph const *, DecisionList const * decisions);
	~LpMaster();
public:
	void freeLp();
	void initLp();

	virtual void add(Column const & column);
	void add(std::set<Column> const & columns);
	void add(Column const &, ColumnBuffer &) const;

	virtual void add(std::set<Column> const & columns, size_t & nb, Double&rd);

	void add(Column const & column, ColumnBuffer & columnBuffer,
			size_t current_n, Double & rd, size_t &nb);

	virtual void add(ReducedCostSorter const & columns, size_t, size_t & nb,
			Double&rd);

	void add(ModularityBPartition const & column);
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

	void applyBranchingRule();
	void branchingWeights(FractionnarySolution const &, BranchingWeights &);

	void build();
private:
	CPXENVptr _env;
	CPXLPptr _lp;

	Double _obj;
};

#endif 
