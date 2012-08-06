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
#include "IGraclus.hpp"
#include "Timer.hpp"
#include "NormalizedCut.hpp"

int main(int argc, char ** argv) {
	NormalizedCut cut;
	std::cout << cut.isPartitioning() << "\n";
	Timer total;
	Timer t;
	Number::SetSeed(0);
	if (argc == 1)
		return 0;
	std::string graphFileName(argv[1]);
	Graph graph(graphFileName);
	int const k = 32;
	// utilisation de graclus
	IGraclus * iGraclus(IGraclus::Get());
	iGraclus->allocate(graph.nbNodes(), graph.nbEdges(), false);
	for (size_t i(0); i < graph.nbNodes(); ++i)
		iGraclus->setRow(i, graph.adjacentList(i));
	iGraclus->check();
	iGraclus->launch(32);
	std::cout << "Init    time " << t.elapsed() << "\n";
	t.restart();
	iGraclus->launch(k);
	std::cout << "Launch  time " << t.elapsed() << "\n";
	std::cout << "Total   time " << total.elapsed() << "\n";
	std::cout << "Value        " << iGraclus->score() << "\n";

	delete iGraclus;

	//	Data data(graph);
	//
	//	IntVector x(Random(graph.nbNodes()));
	//	data.startWith(x);
	//
	//	ICriterion * criterion = new Modularity;
	//	ICriterion * criterion = new Density;

	//	Vnds vnds;
	//	vnds.run(data, *criterion, 10);

	//	delete criterion;
}
