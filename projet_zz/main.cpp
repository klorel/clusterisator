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


#ifdef _WIN64
	#define CMD_SUPP 0
#elif _WIN32
	#define CMD_SUPP 0
#elif __APPLE__
	#define CMD_SUPP  1
#elif __linux
	#define CMD_SUPP  1
#endif



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
			// Ã©crire dans un fichier les rÃ©sultats : moyennÃ© sur tous les problÃ¨mes
			// vÃ©rifier la cohÃ©rence on ne peut dÃ©tÃ©riorer la solution de dÃ©part
			// KMEANS : KMEANS classique
			// on souhaite se comparer au KMEANS normal 
			// - % temps total dans le KMEANS (sans construction des instances intermÃ©diaires)
			// - % nombre d'itÃ©ration en plus KMEANS
			// - % temps moyen des itÃ©rations
			// - l'Ã©cart par rapport au KMEANS normal

			AvailableInstances id(static_cast<AvailableInstances>(i));
			RegisteredInstance instance(id);
			instance.out();
			
			
			MultiLevelAlgo algo(instance, k);


			algo.buildMultiLevelData(20*k,20);
			Partition start(instance.nbObs(), k);
			// gÃ©nÃ©ration du point de dÃ©part
			algo.getStartPoint(start);
			algo.setStartPoint(start);
			// ----s
			size_t level=0;
			algo.setStep(1);
			bool fini = false;
			int nbfichier=1;

			if (CMD_SUPP == 0)
				system("rmdir /S /Q Run");
			else
				system("rm -r Run");
			system(" mkdir Run");
			while(!fini){



				
				
				algo.setfic("./Run/run"+ std::to_string(nbfichier)+".txt");

				algo.initfichier();
				algo.setStartLevel(level);
				algo.launch();
				if (level ==5)
					fini=true;
				level = 5;
				nbfichier++;
			}
		}
	}
	system("pause");
	return 0; 
}
