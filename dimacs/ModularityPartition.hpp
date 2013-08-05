#ifndef MODULARITY_PARTITION_HPP
#define MODULARITY_PARTITION_HPP
#include "gencol.h"
#include "Partition.hpp"
#include "Graph.hpp"

class ModularityPartition : public Partition {
	ModularityPartition(Graph const &, size_t k = 1);
	ModularityPartition(Graph const &, Partition const &);
	virtual ~ModularityPartition();

	Double computeScore()const;
private:
	Graph const & _input;
};

#endif /* GRAPH_HPP */
