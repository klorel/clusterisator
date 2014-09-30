/*
 * UnipartieInstance.cpp
 *
 *  Created on: 6 mars 2014
 *      Author: manuel
 */

#include "UnipartieInstance.h"
#include "Column.hpp"

UnipartieInstance::UnipartieInstance() {
	// TODO Auto-generated constructor stub

}

UnipartieInstance::~UnipartieInstance() {
	// TODO Auto-generated destructor stub
}

void UnipartieInstance::writeSolution(FractionnarySolution const& bestSolution,
		double lb) const {
	std::ofstream file(GetStr("optimal/", problemName(), "_", lb, ".txt").c_str());
	for (auto const & c : bestSolution) {
		for (size_t r(0); r < nV(); ++r) {
			for (size_t b(r + 1); b < nV(); ++b) {
				if (c.first->contains(r) && c.first->contains(b)) {
					file << std::setw(6) << 1 + r;
					file << std::setw(6) << 1 + b;
					file << std::endl;
				}

			}
		}
	}
	file.close();
}
