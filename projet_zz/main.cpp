/*
 * main.cpp
 *
 *  Created on: 15 dÃ©c. 2012
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

void usage() {
	std::cout << "Available instances : \n";
	for (size_t i(0); i < AvailableInstances::SIZE; ++i) {
		AvailableInstances id(static_cast<AvailableInstances>(i));
		RegisteredInstance instance(id);
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
	Number::SetSeed(1);
	//liste des instances a tester
	std::list<size_t> list_instance;
	//	list_instance.push_back(0);
	//list_instance.push_back(1);
	//list_instance.push_back(4);
	//list_instance.push_back(7);
	list_instance.push_back(8);
	//list_instance.push_back(9);
	//list_instance.push_back(12);
	//list_instance.push_back(15);
	//list_instance.push_back(16);
	//list_instance.push_back(19);
	//list_instance.push_back(22);
	//list_instance.push_back(25);
	//list_instance.push_back(28);
	//list_instance.push_back(29);

	// prÃ©paration de l'output qu iszera effacer Ã  chaque run !!

	std::ofstream file("output.csv");

	file << "id;nom;n;k;step;amax;nbLevel; ";
	file << "ite0;score0;CPU0; ";
	file << "start;";
	file << "ite;score;CPU;";
	//file << "Rapport Scores (%);Rapport iterations total(%); rapport iterations moyen(%);Rapport temps(%)";
	file << std::endl;
	// ce programme lance les tests sur toutes les instances.
	std::ofstream debug("debug.log");
	for (auto const & i : list_instance) {
		AvailableInstances id(static_cast<AvailableInstances>(i));
		RegisteredInstance instance(id);
		instance.out();
		// on va tester notre algo pour un nombre de classe compris [10 , 15% Nbpoint] en incrementant de 1
		size_t const kmax((size_t) std::ceil(instance.nbObs() * 0.15));
		size_t const amax((size_t) std::ceil(instance.nbObs() * 0.10));
		std::cout << "amax = " << amax << std::endl;
		// mais on arrete si notre algo n'est plus en multi-level.
		for (size_t k(10); k < kmax; ++k) {
			MultiLevelAlgo algo(instance, k);
			algo.setOut(debug);
			// on agrege 20k des noeuds par palier de amax 
			algo.buildMultiLevelData(20 * k, amax);
			Partition start(instance.nbObs(), k);
			//generation du point de depart
			algo.getStartPoint(start);

			size_t const stepMax((size_t) std::ceil(algo.nbLevels() * 0.10));
			
			std::map<size_t, MultiLevelAlgoStats> allStats;
			std::map<size_t, double> sumsIte;
			std::map<size_t, size_t> nbIte;
			std::map<size_t, double> score;
			std::map<size_t, double> sumsTime;


			//On fait varier le pas de 1 à stepMax+1 avec stepMax qui est 10% du nb de niveau
			for (size_t step(1) ; step < stepMax+1 ; ++step)
			{
				algo.setStep(step);

				for (size_t level(0); level <= algo.nbLevels(); ++level) {
					algo.setStartPoint(start);
					algo.setStartLevel(level);
					algo.launch();
					allStats[level] = algo.stats();
				}

				for (auto const & stat : allStats) {
					size_t const level(stat.first);
					nbIte[level] = allStats[level].size();
					score[level] = allStats[level].rbegin()->second._cost;
					for (auto & stat : allStats[level]) {
						sumsIte[level] += stat.second._ite;
						sumsTime[level] += stat.second._time;
					}
				}
				for (auto const & stat : allStats) {
					size_t const level(stat.first);
					//double RapCost	( (scoreTot[level]-sumsCost[algo.nbLevels()])/sumsCost[algo.nbLevels()]);
					//double RapIte	( (sumsIteTot[level]-sumsIteTot[algo.nbLevels()])/sumsIteTot[algo.nbLevels()]);
					//double RapTime	( (sumsTimeTot[level]-sumsTimeTot[algo.nbLevels()])/sumsTimeTot[algo.nbLevels()]);
					//double RapIteMoy( (sumsIte[level]-sumsIte[algo.nbLevels()])/sumsIte[algo.nbLevels()]);

					WriteCsv(file, i, 6);
					WriteCsv(file, instance.name);
					WriteCsv(file, instance.nbObs());
					WriteCsv(file, k);
					WriteCsv(file, step);
					WriteCsv(file, amax);
					WriteCsv(file, algo.nbLevels());
					WriteCsv(file, sumsIte[0]);
					WriteCsv(file, score[0], 15);
					WriteCsv(file, sumsTime[0], 6);
					WriteCsv(file, level);
					WriteCsv(file, sumsIte[level]);
					WriteCsv(file, score[level], 15);
					WriteCsv(file, sumsTime[level], 6);
					file << std::endl;
				}
			}
		}
	}
	file.close();
	debug.close();
	//	std::cout << "Press ENTER to continue...";
	//	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return 0;
}
