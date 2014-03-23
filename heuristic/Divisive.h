/*
 * Divisive.h
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#ifndef DIVISIVE_H_
#define DIVISIVE_H_

#include "ModularityBPartition.hpp"

class Divisive {
public:
	Divisive(BipartiteGraph & instance, ModularityBPartition & solution);
	virtual ~Divisive();
public:
	void run();
private:
	BipartiteGraph & _instance;
	ModularityBPartition & _solution;
};

#endif /* DIVISIVE_H_ */
