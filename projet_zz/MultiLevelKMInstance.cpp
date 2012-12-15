/*
* MultiLevelKMInstance.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/

#include "../projet_zz/MultiLevelKMInstance.h"
#include "../src/HMeans.hpp"
#include "../src/KMAlgo.hpp"
#include "../src/Timer.hpp"
MultiLevelAlgo::MultiLevelAlgo(KMInstance const & instance, size_t k) :
	_instance(instance), _input(_instance, k) {
}
MultiLevelAlgo::~MultiLevelAlgo() {
	for (auto & ptr : _multiLevelConstraints)
		delete ptr;
}
void MultiLevelAlgo::buildInstance(size_t level, KMInstance & instance,Aggregations & aggregations) {
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
	for(size_t p(0); p<nbNodes; ++p){
		_multiLevelConstraints.push_back(new KMConstraints(_input.nbObs()));
		size_t const step(20);
		for(size_t q(1);q<step; ++q){ 
			_multiLevelConstraints.back()->newCtr(step*p, step*p+q);
		}
	}
}
//
void MultiLevelAlgo::refine() {
	// lancer le KMEANS sur chaque niveau en partant du plus élevé (celui qui contient le moins de noeuds)
	// à chaque fois on initialise avec le niveau précédent (sauf le premier!)
	// Pour le premier faire un appel à random(0);
	KMInstance instance;
	Aggregations aggregations;	
	Timer timer;
	// pour chaque level
	for (size_t level(0); level <= _multiLevelConstraints.size(); ++level) {
		// ! on parcours à l'envers
		buildInstance(_multiLevelConstraints.size() - level, instance,aggregations);
		KMInput input(instance, _input.maxNbLabels());
		// initialiser cette input avec la solkution courante
		// attention il faut utiliser aggregation pour faire les neodus agrégés et la solution courante
		if(level==0){
			input.random(0);
			for (size_t i(0); i < _input.nbObs(); ++i) {
				_input.shiftForced(i, input.label(aggregations.newIds[i]));
			}			
		}else{
			for (size_t i(0); i < _input.nbObs(); ++i) {
				input.shiftForced(aggregations.newIds[i], _input.label(i));
			}
		}
		// on lance l'algo
		HMeans<false>()(input);
		std::cout << std::setw(10)<<_multiLevelConstraints.size() - level;
		std::cout << std::setw(10)<<input.ite();
		std::cout << std::setw(10)<<timer.elapsed();
		std::cout << std::setw(20)<<input.cost();
		std::cout << std::endl;
		// suavegarde de la solution
		input.computeCenters();
		for (size_t i(0); i < _input.nbObs(); ++i) {
			_input.shiftForced(i, input.label(aggregations.newIds[i]));
		}
		for (size_t i(0); i < _input.nbObs(); ++i) {
			input.shiftForced(aggregations.newIds[i], _input.label(i));
		}
	}
}
void MultiLevelAlgo::launch(size_t nbNodes) {
	buildMultiLevelData(nbNodes);

	refine();

}
