/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "common.h"
#include "Graph.hpp"
#include "Data.hpp"
#include "NormalizedCut.hpp"

#include "Shifter.hpp"
#include "LocalSearch.hpp"

int main(int argc, char ** argv) {

	if (argc != 3) {
		std::cout << "ncut <graphFile> <partFile>\n";
		return 0;
	}
	std::string graphFileName(argv[1]);
	std::string partitionName(argv[2]);

	Graph graph(graphFileName);

	Partition partition(partitionName);

	Data data(graph);
	data.startWith(partition);

	NormalizedCut ncut;
	std::cout << "Ncut value : " << ncut.eval(data) << "\n";
	IndexedList nodes(graph.nbNodes(), true);

	Shifter shifter(data, ncut, nodes, data.used());
	LocalSearch ls(shifter);
	ls.run();
	return 0;
}
