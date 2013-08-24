/*
* main.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/
#ifndef VNS_H__
#define VNS_H__

#include "common.h"
#include "Partition.hpp"
#include "Number.hpp"
#include <random>
#include <iostream>
#include <fstream>
#include "RegisteredModularityBInstance.hpp"
#include "BipartiteGraph.hpp"
#include "ModularityBPartition.hpp"

int vns(int argc, char** argv);
bool optimize(ModularityBPartition & p, Double & score, IntVector & solution);

#endif