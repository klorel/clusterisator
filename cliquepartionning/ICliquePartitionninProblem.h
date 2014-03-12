/*
 * ICliquePartitionninProblem.h
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#ifndef ICLIQUEPARTITIONNINPROBLEM_H_
#define ICLIQUEPARTITIONNINPROBLEM_H_

#include "common.h"

class ICliquePartitionninProblem {
public:
	virtual ~ICliquePartitionninProblem();

	virtual size_t nV()=0;
};

#endif /* ICLIQUEPARTITIONNINPROBLEM_H_ */
