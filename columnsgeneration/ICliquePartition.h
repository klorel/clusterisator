/*
 * ICliquePartion.h
 *
 *  Created on: 14 mars 2014
 *      Author: manuel
 */

#ifndef I_CLIQUE_PARTITION_H_
#define I_CLIQUE_PARTITION_H_

#include "common.h"
class ICliquePartitionProblem;

class ICliquePartition {
public:
	ICliquePartition(ICliquePartitionProblem const &);
	virtual ~ICliquePartition();
public:
	virtual double score() const=0;

	virtual double & score()=0;

	virtual double computeCost() const=0;

	virtual bool shift(size_t obs, size_t to)=0;

	virtual size_t label(size_t obs)=0;

	virtual void checkScore() const = 0;

	virtual ICliquePartitionProblem const & problem() const;

	virtual IndexedList const & usedLabels()const = 0;
private:
	ICliquePartitionProblem const & _problem;
};

inline ICliquePartition::ICliquePartition(
		ICliquePartitionProblem const & problem) :
		_problem(problem) {
}

inline ICliquePartition::~ICliquePartition() {
}

#endif /* ICLIQUEPARTION_H_ */
