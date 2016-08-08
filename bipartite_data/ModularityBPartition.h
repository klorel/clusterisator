#ifndef MODULARITY_B_PARTITION_HPP
#define MODULARITY_B_PARTITION_HPP

#include "../clustering/Partition.h"
#include "BipartiteGraph.h"
#include "common.h"

class ModularityBPartition: public Partition {
public:
	ModularityBPartition(BipartiteGraph const &, int k = 1);
	ModularityBPartition(BipartiteGraph const &, Partition const &);
	ModularityBPartition(BipartiteGraph const *, int k = 1);
	ModularityBPartition(BipartiteGraph const *, Partition const &);
	virtual ~ModularityBPartition();

	Double computeScore() const;
	Double &score();
	Double score() const;
	Double score(int label) const;
	Double scoreIfSwap(int label, int node) const;

	void checkScore() const;

	void init();
private:
	BipartiteGraph const * _input;
	Double _score;
};

#endif /* GRAPH_HPP */
