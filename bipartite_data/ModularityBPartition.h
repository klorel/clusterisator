#ifndef MODULARITY_B_PARTITION_HPP
#define MODULARITY_B_PARTITION_HPP

#include "../clustering/Partition.h"
#include "BipartiteGraph.h"
#include "common.h"

class ModularityBPartition: public Partition {
public:
	ModularityBPartition(BipartiteGraph const &, size_t k = 1);
	ModularityBPartition(BipartiteGraph const &, Partition const &);
	ModularityBPartition(BipartiteGraph const *, size_t k = 1);
	ModularityBPartition(BipartiteGraph const *, Partition const &);
	virtual ~ModularityBPartition();

	Double computeScore() const;
	Double &score();
	Double score() const;
	Double score(size_t label) const;
	Double scoreIfSwap(size_t label, size_t node) const;

	void checkScore() const;

	void init();
private:
	BipartiteGraph const * _input;
	Double _score;
};

#endif /* GRAPH_HPP */
