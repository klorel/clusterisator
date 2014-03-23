#include "ICliquePartitionProblem.h"

ICliquePartitionProblem::~ICliquePartitionProblem() {
}

void ICliquePartitionProblem::adjencyGraph(std::vector<IntSet> & result) const {
	result.resize(nV());
	for (auto & s : result)
		s.clear();
	for (auto const & e : edges()) {
		result[e._i].insert(e._j);
		result[e._j].insert(e._i);
	}

}
