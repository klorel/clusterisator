/*
 * Divisive.cpp
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#include "Divisive.h"

#include "Divisor.hpp"

namespace bipartite {
Divisive::Divisive(BipartiteGraph & instance, ModularityBPartition & solution) :
		_instance(instance), _solution(solution) {
	// TODO Auto-generated constructor stub

}

Divisive::~Divisive() {
	// TODO Auto-generated destructor stub
}
void Divisive::run() {
	Divisor divisor(_instance);
	IndexedList usedLabels;
	IndexedList skipedLabels(_solution.usedLabels());
	skipedLabels.clear();
	bool stop(false);
	while (!stop) {
		std::cout << "NEW LOOP" << std::endl;
		stop = true;
		usedLabels = _solution.usedLabels();
		for (auto const & label : usedLabels) {
			if (!skipedLabels.contains(label)) {
				if (divisor.run(_solution, label))
					stop = false;
				else {
					skipedLabels.insert(label);
				}
				std::cout << std::setw(4) << label;
				std::cout << std::setw(20) << std::setprecision(10)
						<< _solution.score();
				std::cout << std::endl;
			}
		}
	}
	std::cout << "DIVISIVE END" << std::endl;

}
}
