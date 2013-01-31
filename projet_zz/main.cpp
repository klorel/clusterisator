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
			// écrire dans un fichier les résultats : moyenné sur tous les problèmes
			// vérifier la cohérence on ne peut détériorer la solution de départ
			// KMEANS : KMEANS classique
			// on souhaite se comparer au KMEANS normal 
			// - % temps total dans le KMEANS (sans construction des instances intermédiaires)
			// - % nombre d'itération en plus KMEANS
			// - % temps moyen des itérations
			// - l'écart par rapport au KMEANS normal

			AvailableInstances id(static_cast<AvailableInstances>(i));
			RegisteredInstance instance(id);
			instance.out();
			MultiLevelAlgo algo(instance, k);
			
			algo.buildMultiLevelData(0,0);
			Partition start(instance.nbObs(), k);
			// génération du point de départ
			algo.getStartPoint(start);
			// ----
			size_t level(0);
			algo.setStep(1);
			while(true){
				algo.setStartLevel(level);
				algo.launch(20*k);
			}
		}
	}
	system("pause");
	return 0; 
}
