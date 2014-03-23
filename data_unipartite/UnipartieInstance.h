/*
 * UnipartieInstance.h
 *
 *  Created on: 6 mars 2014
 *      Author: manuel
 */

#ifndef UNIPARTIEINSTANCE_H_
#define UNIPARTIEINSTANCE_H_

#include "Edge.h"

class UnipartieInstance {
public:
	UnipartieInstance();
	virtual ~UnipartieInstance();
private:
	Edges _edges;
};

#endif /* UNIPARTIEINSTANCE_H_ */
