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

#include "Number.hpp"
#include "Density.hpp"
#include "IGraclus.hpp"
#include "Timer.hpp"
#include "NormalizedCut.hpp"

int main(int argc, char ** argv) {

	Timer total;
	Timer timer;
	Number::SetSeed(0);
	if (argc != 3) {
		std::cout << "<exe> <graphFile> <nb partition>\n";
		return 0;
	}
	std::string graphFileName(argv[1]);
	Graph graph(graphFileName);
	int const k = atoi(argv[2]);
	// utilisation de graclus
	IGraclus * iGraclus(IGraclus::Get());
	iGraclus->allocate(graph.nbNodes(), graph.nbEdges(), false);
	for (size_t i(0); i < graph.nbNodes(); ++i)
		iGraclus->setRow(i, graph.adjacentList(i));
	iGraclus->check();
	iGraclus->launch(32);
	std::cout << "Init    time " << timer.elapsed() << "\n";
	timer.restart();
	iGraclus->launch(k);
	std::cout << "Launch  time " << timer.elapsed() << "\n";
	std::cout << "Total   time " << total.elapsed() << "\n";
	std::cout << "Value        " << iGraclus->score() << "\n";
	std::ofstream file((graphFileName + ".part." + argv[2]));
	for (size_t i(0); i < graph.nbNodes(); ++i)
		file << iGraclus->get(i) << "\n";
	file.close();
	delete iGraclus;

	return 0;
}
