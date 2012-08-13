/*
 * LPA2.hpp
 *
 *  Created on: 22 juil. 2011
 *      Author: manuel
 */

#ifndef DATA_HPP_
#define DATA_HPP_

#include "../utils/common.h"

#include "../utils/IndexedList.hpp"
#include "../interfaces/ISolution.hpp"
#include "Graph.hpp"
#include "Partition.hpp"

class Solution: public ISolution {
public:
public:
	Solution(Problem const &, IPartition & partition);
	virtual ~Solution();
public:
	// déplacement d'un noeud
	void shift(size_t n, size_t p);
	// fusion de deux labels
	size_t fusion(size_t l1, size_t l2);
	//
public:
	Double computeSumOfLabel(size_t l) const;
	void computeSumOfLabels();

	bool check() const;
	bool checkPosition() const;
	bool checkDegrees() const;

	bool checkDegree(size_t const l) const;

};



#endif /*DATA_HPP_ */
