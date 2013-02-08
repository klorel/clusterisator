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

int main(int argc, char ** argv) {
	Number::SetSeed(1);
	if (argc < 3) {
		usage();
	} else {
		size_t const i(atoi(argv[1]));
		size_t const k(atoi(argv[2]));
		if (i < AvailableInstances::SIZE) {
			// Ecrire dans un fichier les resultats : moyenne sur tous les problemes
			// verifier la coherence on ne peut deteriorer la solution de depart
			// KMEANS : KMEANS classique
			// on souhaite se comparer au KMEANS normal 
			// - % temps total dans le KMEANS (sans construction des instances intermediaires)
			// - % nombre d'iteration en plus KMEANS
			// - % temps moyen des iterations
			// - l'ecart par rapport au KMEANS normal

			AvailableInstances id(static_cast<AvailableInstances>(i));
			RegisteredInstance instance(id);
			instance.out();			
			MultiLevelAlgo algo(instance, k);
			algo.buildMultiLevelData(20*k, std::ceil(instance.nbObs()/4.0));
			Partition start(instance.nbObs(), k);
			 //generation du point de depart
			algo.getStartPoint(start);
            
			// ----s
			bool done(false);
			std::ofstream file("output.csv");
			algo.setOut(file);
			//algo.out() << "
			// pour nos tests on va crÃ©er une liste sur le premier niveau a traiter et une deuxieme pour les sauts.
			// MR : commencer par faire sans saut .... !!!!
			algo.setStep(1);
			std::vector<MultiLevelAlgoStats> allStats(algo.nbLevels()+1);
			for(size_t level(0); level<=algo.nbLevels(); ++level){
				algo.setStartPoint(start);
				algo.setStartLevel(level);
				algo.launch();
				allStats[level] = algo.stats();
			}
			/*
			std::vector<double> sumsIte(algo.nbLevels()+1);
			std::vector<double> sumsTime(algo.nbLevels()+1);
			std::vector<double> sumsCost(algo.nbLevels()+1);
			//*/
			for (size_t level(0); level<=algo.nbLevels(); ++level)
			{
				double sumIte = 0.0 , sumTime = 0.0 , sumCost = 0.0 ;
				size_t cpt = 0 ;
				for (auto & stat : allStats[level]) {
					sumIte += (double) stat.second._ite ;
					sumTime += stat.second._time ;
					sumCost += stat.second._cost ;
					cpt++ ;
				}
				/*			
				sumsIte[level] = sumIte / ((double) cpt) ;
				sumsTime[level] = sumTime / ((double) cpt) ;
				sumsCost[level] = sumCost / ((double) cpt) ;
				//*/
				algo.out() << "_________________________________________________________" << std::endl ;
				algo.out() << "Niveau de départ : " << level << std::endl ;
				algo.out() << "Nombre moyen d'itérations :" << sumIte / ((double) cpt)  << std::endl ;
				algo.out() << "Temps moyen des k-moyennes :" << sumTime / ((double) cpt)  << std::endl ;
				algo.out() << "Temps total des k-moyennes :" << sumTime  << std::endl ;
				algo.out() << "Coût moyen :" << sumCost / ((double) cpt)  << std::endl ;
				algo.out() << "Coût final :" << allStats[level][(--(allStats[level].end()))->first]._cost  << std::endl ;
				algo.out() << "_________________________________________________________" << std::endl ;
			}
			file.close();
		}
	}
	
	return 0; 
}
