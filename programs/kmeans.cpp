/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "../src/common.h"
#include "../src/Partition.hpp"

#include "../src/KMAlgo.hpp"
#include "../src/KMInstance.hpp"
#include "../src/KMConstraints.hpp"
#include "../src/KMPartition.hpp"

#include "../src/Env.hpp"
#include "../src/Number.hpp"

/*
 * to do :
 * tirer aléatoirement des partitions agrégés et vérifier les objectifs et delta par rapport au modèle classique
 */

void testDelta(KMAlgo & algo) {
	Timer timer;
	Double const obj(
			KMAlgo::ComputeMssc(algo.partition(), algo.partition().instance()));
	for (size_t i(0); i < algo.partition().nbObs(); ++i) {
		if (i % 100 == 0)
			algo.partition().computeCenters();
		size_t const l(algo.partition().label(i));

		for (size_t k(0); k < algo.partition().nbLabels(); ++k) {
			Double const delta(algo.getDelta(i, k));
			algo.shift(i, k);
			if (!IsEqual(
					KMAlgo::ComputeMssc(algo.partition(),
							algo.partition().instance()), algo.computeCost())) {
				OUT<< std::setw(6)<<timer.elapsed();
				OUT<< std::setw(4)<<i;
				OUT<< std::setw(4)<<l;
				OUT<< std::setw(4)<<k;
				OUT<<"\n";
				OUT<<KMAlgo::ComputeMssc(algo.partition(),
						algo.partition().instance())
				<< " != "<< algo.computeCost()<<"\n";
			}
			if (!IsEqual(obj + delta, algo.computeCost())) {
				OUT<< std::setw(6)<<timer.elapsed();
				OUT<< std::setw(4)<<i;
				OUT<< std::setw(4)<<l;
				OUT<< std::setw(4)<<k;
				OUT<<"\n";
				OUT<< obj+delta << " != "<<algo.computeCost()<<"\n";
				assert(false);
			}
			algo.shift(i, l);
		}
	}
}

int main(int argc, char ** argv) {

	std::string const dataFileName(argv[1]);
	size_t const k(atoi(argv[2]));

	Number::SetSeed(0);
//	Number::SetSeed(argc > 3 ? atoi(argv[3]) : 0);

	KMInstance instance;
	instance.readData(dataFileName);
	if (argc > 3) {
		std::string const ctrsFileName(argv[3]);
		instance.readConstraints(ctrsFileName);
	}

	Agregations agregations;

	instance.buildMustLink(agregations);
	// agrégation
	KMInstance instance2(instance, agregations);
	KMPartition partition2(instance2, k);
	partition2.random(k);
	KMAlgo kmeans2(partition2);

	std::cout << std::setprecision(15) << kmeans2.computeCost() << "\n";
	kmeans2.hMeans(600);
	return 0;
}
