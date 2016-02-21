#ifndef LP_MASTER_HPP
#define LP_MASTER_HPP

#include "Column.h"
#include "Decision.h"
#include "gencol.h"
#include "IMaster.h"

class Node;
typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class ColumnBuffer;
class LpMaster: public IMaster {
public:
	LpMaster(ICliquePartitionProblem const *, DecisionList const * decisions);
	~LpMaster();
public:
	void freeLp();
	void initLp();
	virtual void add(IPartition const & solution);
	virtual void add(Column const & column);
	void add(std::set<Column> const & columns);
	void add(Column const &, ColumnBuffer &) const;

	virtual void add(std::set<Column> const & columns, size_t & nb, Double&rd);

	void add(Column const & column, ColumnBuffer & columnBuffer,
			size_t current_n, Double & rd, size_t &nb);

	virtual void add(ReducedCostSorter const & columns, size_t, size_t & nb,
			Double&rd);

	void addSingleton();

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
//	void branchingWeights(FractionnarySolution const &, BranchingWeights &);

	void build();
private:
	CPXENVptr _env;
	CPXLPptr _lp;

	Double _obj;
};

#endif 
