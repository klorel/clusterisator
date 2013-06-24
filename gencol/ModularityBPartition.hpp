#ifndef MODULARITY_B_PARTITION_HPP
#define MODULARITY_B_PARTITION_HPP
#include "gencol.h"
#include "Partition.hpp"
#include "BipartiteGraph.hpp"

class ModularityBPartition : public Partition {
public:
	ModularityBPartition(BipartiteGraph const &, size_t k = 1);
	ModularityBPartition(BipartiteGraph const &, Partition const &);
	virtual ~ModularityBPartition();

	Double computeScore()const;
	BipartiteGraph const & input()const;
private:
	BipartiteGraph const & _input;
};

#endif /* GRAPH_HPP */
