/*
 * VnsLabelPropagation.h
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#ifndef VNSLABELPROPAGATION_H_
#define VNSLABELPROPAGATION_H_

#include "common.h"
#include "ModularityBPartition.h"

class ModularityBPartition;

namespace bipartite {
class VnsLabelPropagation {
public:

	void singletize(size_t i, IndexedList & allNodes,
			ModularityBPartition & solution);

	void set(IntVector const & l, ModularityBPartition & solution);

	void run();
public:
	VnsLabelPropagation(BipartiteGraph & instance,
			ModularityBPartition & solution, size_t kMax, size_t iteMax);
	virtual ~VnsLabelPropagation();
private:
	BipartiteGraph & _instance;
	ModularityBPartition & _solution;
	size_t _kMax;
	size_t _iteMax;
};
}
#endif /* VNSLABELPROPAGATION_H_ */
