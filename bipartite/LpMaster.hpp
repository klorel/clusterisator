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

	void add(std::set<Column> const & columns);
	void add(std::set<Column> const & columns, size_t & nb, Double&rd);

	void add(ReducedCostSorter const & columns, size_t, size_t & nb, Double&rd);

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

	void applyBranchingRule();
	void branchingWeights(FractionnarySolution const &, BranchingWeights &);

	void build();

	void buildDualBounds(ModularityBPartition const &);

	void buildStabilization(ModularityBPartition const &);
	bool stabilized() const;

	bool updateStabilization();
	bool centerStabilization();

	void resetStabilization();

	void stabilizationStat(std::ostream & = std::cout) const;

	size_t stabilizationUpdates() const;
private:
	BipartiteGraph const * _input;
	CPXENVptr _env;
	CPXLPptr _lp;

	std::set<Column> _columns;
	std::vector<double> _dual;
	std::vector<double> _primal;

	Double _obj;
	DecisionList const * _decisions;
	/*
	 * stabilization
	 */
	std::vector<double> _dualLower;
	std::vector<double> _dualUpper;

	bool _stabilized;
	std::vector<int> _yIndex;
	std::vector<int> _epsIndex;
	std::vector<double> _yCost;
	Double _stabilizationCost;
	size_t _stabilizationUpdates;

//	std::vector<std::vector<std::list<Column const *> > > _rAndbInColumn;
//	std::vector<std::vector<std::list<Column const *> > > _rOrbInColumn;

};

#endif 
