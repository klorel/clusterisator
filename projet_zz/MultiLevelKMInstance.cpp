/*
 * MultiLevelKMInstance.cpp
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */

#include "../projet_zz/MultiLevelKMInstance.h"
#include "../src/HMeans.hpp"
#include "../src/KMAlgo.hpp"

MultiLevelAlgo::MultiLevelAlgo(KMInstance const & instance, size_t k) :
		_instance(instance), _input(_instance, k) {
}
MultiLevelAlgo::~MultiLevelAlgo() {
	for (auto & ptr : _multiLevelConstraints)
		delete ptr;
}
void MultiLevelAlgo::buildInstance(size_t level, KMInstance & instance,
		Aggregations & aggregations) {
	// on enlève toutes les contraintes
	_instance.mustLinks().clear();
	_instance.cannotLinks().clear();
	for (size_t i(0); i < level; ++i) {
		for (auto const & ctr : *_multiLevelConstraints[i]) {
			_instance.addMustLink(ctr.first, ctr.second);
		}
	}
	// construit les infos de correspondances entre les instances
	_instance.buildMustLink(aggregations);
	// construit l'instance aggrégée
	instance = KMInstance(_instance, aggregations);
}
// nbNodes : le nombre de noeuds minimal dans le graph de plus bas niveau
// critère défini pour ajouter les noeuds : voisin le plus proche
void MultiLevelAlgo::buildMultiLevelData(size_t nbNodes) {
	// écrire l'algorithme d'aggrégation
	_multiLevelConstraints.push_back(new KMConstraints(_input.nbObs()));
	for (size_t i(1); i < nbNodes; ++i) {
		_multiLevelConstraints.back()->newCtr(0, i);
	}
	_multiLevelConstraints.push_back(new KMConstraints(_input.nbObs()));
	for (size_t i(nbNodes); i < nbNodes * 2; ++i) {
		_multiLevelConstraints.back()->newCtr(nbNodes, nbNodes + i);
	}
}
//
void MultiLevelAlgo::refine() {
	// lancer le KMEANS sur chaque niveau en partant du plus élevé (celui qui contient le moins de noeuds)
	// à chaque fois on initialise avec le niveau précédent (sauf le premier!)
	// Pour le premier faire un appel à random(0);

	KMInstance instance;
	Aggregations aggregations;
	// pour chaque level
	for (size_t level(0); level <= _multiLevelConstraints.size(); ++level) {
		// ! on parcours à l'envers
		buildInstance(_multiLevelConstraints.size() - level, instance,
				aggregations);
		std::cout << instance.cst() << std::endl;
		KMInput input(instance, _input.nbLabels());

		// initialiser cette input avec la solkution courante
		// attention il faut utiliser aggregation pour faire les neodus agrégés et la solution courante
		for (size_t i(0); i < _input.nbObs(); ++i) {
			input.shift(aggregations.newIds[i], _input.label(i));
		}

		// on lance l'algo
		HMeans<true>()(input);
		// suavegarde de la solution
		for (size_t i(0); i < _input.nbObs(); ++i) {
			_input.shift(i, input.label(aggregations.newIds[i]));
		}
	}
}
void MultiLevelAlgo::launch(size_t nbNodes) {
	buildMultiLevelData(nbNodes);
	// initialisation at random
	_input.random(0);
	//
	refine();

}
