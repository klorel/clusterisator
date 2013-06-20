#ifndef MODULARITY_PARTITION_HPP
#define MODULARITY_PARTITION_HPP

#include "src/common.h"
#include "src/Partition.hpp"
#include "gencol/Graph.hpp"

class ModularityPartition : Partition {
	ModularityPartition(Graph const &, size_t k = 1);
	ModularityPartition(Graph const &, Partition const &);
	virtual ~ModularityPartition();

	Double computeScore()const;
private:
	Graph const & _input;
};

#endif /* GRAPH_HPP */
