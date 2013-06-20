/*
* main.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/

#include "src/common.h"
#include "src/Partition.hpp"

#include "src/KMAlgo.hpp"
#include "src/KMInstance.hpp"
#include "src/KMConstraints.hpp"
#include "src/KMPartition.hpp"

#include "src/Number.hpp"
#include "src/RegisteredInstance.hpp"

#include "projet_zz/MultiLevelKMInstance.h"
#include <random>
#include <iostream>
#include <fstream>


double CalculSeuil(double ,RegisteredK:Instance );


void usage() {
	std::cout << "Available instances : \n";
	for (size_t i(0); i < AvailableInstances::SIZE; ++i) {
		AvailableInstances id(static_cast<AvailableInstances>(i));
		RegisteredK:Instance instance(id);
		std::cout << std::setw(3) << i;
		std::cout << " : ";
		std::cout << std::setw(30) << std::left << instance.name << std::right;

		std::cout << "\n";
	}
	std::cout << "<exe> <id of selected instance> <number of labels>\n";
	std::cout << "The program launch the multi level algorithm\n";

}
template<class T> void WriteCsv(std::ostream & stream, T const & t) {
	stream << t << ";";
}
template<class T> void WriteCsv(std::ostream & stream, T const & t, size_t n) {
	stream << std::setprecision(n) << t << ";";
}

int main(int argc, char ** argv) {
	//usage();
	//liste des instances a tester
	std::list<AvailableInstances> list_instance;
	list_instance.push_back(AvailableInstances::segmentation);

	// préparation de l'output qu iszera effacer à chaque run !!

	std::ofstream file("output.csv");
	std::ofstream stats("Stats.csv");

	file << "id;nom;n;k;Seed;nbLevel;";
	file << "ite0;score0;CPU0;";
	file << "start;";
	file << "ite;score;CPU;";
	//file << "Rapport Scores (%);Rapport iterations total(%); rapport iterations moyen(%);Rapport temps(%)";
	file << std::endl;
	// ce programme lance les tests sur toutes les instances.
	std::ofstream debug("debug.log");

	size_t const nbLancer ( 100);
	
	
	size_t const kmin (3 );
	size_t const kstep( 1 );
	size_t const kmax ( 10);

	for (auto const & id : list_instance) {
		RegisteredK:Instance instance(id);
		instance.out();
		// mais on arrete si notre algo n'est plus en multi-level.
		size_t seed(1);
		Number::SetSeed(seed);
		for (size_t k(kmin); k <= kmax; k+= kstep) {		
			std::map<size_t, MultiLevelAlgoStats> allStats;
			std::map<size_t, size_t> sumIte;
			std::map<size_t, double> sumsTime;
			std::map<size_t, double> score;

			std::cout << " ------------------------"<<k<<std::endl;
			MultiLevelAlgo algo(instance, k);
			//algo.setSeuil(seuil);				
			algo.setOut(debug);
			algo.buildMultiLevelData_tab(20 * k);							
			//algo.buildMultiLevelData_tab(20);
			// on agrege 20k des noeuds par palier de 5% des noeuds totaux 
			//algo.buildMultiLevelData(20 * k, amax);
			Partition start(instance.nbObs(), k);
			//generation du point de depart
			algo.getStartPoint(start);		
			//on ne s'occupe aps encore du pas
			//size_t const stepMax((size_t) std::ceil(algo.nbLevels() * 0.10));
			for (size_t level(0); level <= algo.nbLevels(); ++level) {
				algo.setStartPoint(start);
				algo.setStartLevel(level);
				algo.launch();
				allStats[level] = algo.stats();
				// calcul des stats				
				sumIte[level] = 0;
				sumsTime[level] = 0;
				for(auto const & it : algo.stats()){
					sumIte[level] += it.second._ite;
					sumsTime[level] += it.second._time;					
				}
				score[level] = algo.stats().begin()->second._cost;
			}
			for (size_t level(0); level <= algo.nbLevels(); ++level) {				
				WriteCsv(file, id, 6);
				WriteCsv(file, instance.name);
				WriteCsv(file, instance.nbObs());
				WriteCsv(file, k);
				WriteCsv(file, seed);
				WriteCsv(file, algo.nbLevels());

				WriteCsv(file, sumIte[0]);
				WriteCsv(file, score[0], 15);
				WriteCsv(file, sumsTime[0], 6);
				WriteCsv(file, level);
				WriteCsv(file, sumIte[level]);
				WriteCsv(file, score[level], 15);
				WriteCsv(file, sumsTime[level], 6);
				file << std::endl;
			}
		}
	}
	file.close();
	stats.close();
	debug.close();
	//	std::cout << "Press ENTER to continue...";
	//	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return 0;
}
