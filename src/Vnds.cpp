/*
 * Vnds.cpp
 *
 *  Created on: 18 juil. 2012
 *      Author: manuel
 */

#include "Vnds.hpp"
#include "Modularity.hpp"

Vnds::Vnds() {

}

Vnds::~Vnds() {
}

void Vnds::subProblem(Data const & data, size_t const & size,
		IndexedList & selected) {
	selected.clear();
	IndexedList buffer(data.used());
	while (selected.size() < size && !buffer.empty()) {
		selected.insert(buffer.pop_random());
	}
}

void Vnds::run(Data & data, ICriterion const & criterion,
		size_t const & size_max) {
	MultiStart ms(data, criterion);
	IndexedList selected(data.nbNodes());
	ms.nbEmptyLabel() = 1;
	size_t size(0);
	size_t ite(0);
	do {
		++ite;
		++size;
		if (size > size_max)
			size = 1;
		subProblem(data, size, selected);
		ms.init(selected);
		if (ms.run(1000)) {
			std::cout << std::setw(8) << ite;
			std::cout << std::setw(8) << size;
			std::cout << std::setw(8) << data.nbLabels();
			std::cout << std::setw(20) << ms.score();
			std::cout << "\n";
			size = 0;
		}
	} while (true);
}
