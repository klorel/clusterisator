/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "common.h"
#include "Graph.hpp"
#include "Data.hpp"
#include "Modularity.hpp"
#include "MultiStart.hpp"
#include "Number.hpp"
#include "Vnds.hpp"
#include "Density.hpp"

#undef NDEBUG

int main(int argc, char ** argv) {
	Number::SetSeed(0);
	if (argc == 1)
		return 0;
	std::string graphFileName(argv[1]);
	Graph graph(graphFileName);

	Data data(graph);

	IntVector x(Random(graph.nbNodes()));
	data.startWith(x);

	ICriterion * criterion = new Modularity;
//	ICriterion * criterion = new Density;

	Vnds vnds;
	vnds.run(data, *criterion, 10);

	delete criterion;
}
