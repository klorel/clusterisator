#ifndef LP_MASTER_HPP
#define LP_MASTER_HPP

#include "Column.h"
#include "Decision.h"
#include "gencol.h"
#include "IMaster.h"
#include "Stabilisator.h"
#include "../mip_solver/CplexSolver.h"

class Node;
typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;
class ColumnBuffer;

class PrimalPredicate {
public:
	bool operator()(DoubleVector const & v1, DoubleVector const & v2) const;
	bool operator()(Int2Double const & v1, Int2Double const & v2) const;
};

class LpMaster: public IMaster {
public:
	LpMaster(ClusteringProblem const *, DecisionList const * decisions);
	~LpMaster();
public:
	void initLp();
	virtual void add(IPartition const & solution);
	virtual void add(Column const & column);
	void add(ColumnSet const & columns);
	void add(Column const &, ColumnBuffer &) const;

	virtual void add(ColumnSet const & columns, int & nb, Double&rd);

	void add(Column const & column, ColumnBuffer & columnBuffer, int current_n, Double & rd, int &nb);

	virtual void add(ReducedCostSorter const & columns, int, int & nb, Double&rd);

	void addSingleton();

	void write(std::string const & fileName = "master.lp") const;

	void readColumns(std::string const & fileName);

	void getSolution();
	bool getSolution(FractionnarySolution &);

	void solveMaster();
	void writeColumns(std::string const & = "columns.txt") const;
	Double obj() const;

	DoubleVector const & primal() const;
	DoubleVector const & minus_dual() const;
	ColumnSet const & columns() const;

	void applyBranchingRule();
//	void branchingWeights(FractionnarySolution const &, BranchingWeights &);

	void build();

	Double stabilizationPenalty();
	void udpate_stabilization();
private:
//	CPXENVptr _env;
//	CPXLPptr _lp;

	CplexSolver _solver;

	Double _obj;

	std::map<Int2Double, std::set<IntSet>, PrimalPredicate> _allBasis;

	Stabilisator _stabilisator;
};

#endif 
