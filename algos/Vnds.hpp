/*
 * Vnds.hpp
 *
 *  Created on: 18 juil. 2012
 *      Author: manuel
 */

#ifndef VNDS_HPP_
#define VNDS_HPP_

#include "common.h"
#include "Graph.hpp"
#include "MultiStart.hpp"
#include "Data.hpp"

class Vnds {
public:
	Vnds();
	virtual ~Vnds();

	void subProblem(Data const & data, size_t const & size,
			IndexedList & selected);

	void run(Data & g, ICriterion const &, size_t const & size_max);
};

#endif /* VNDS_HPP_ */
