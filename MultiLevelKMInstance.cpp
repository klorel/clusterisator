/*
* MultiLevelKMInstance.cpp
*
*  Created on: 15 dÃ©c. 2012
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
	// on enlÃ¨ve toutes les contraintes
	_instance.mustLinks().clear();
	_instance.cannotLinks().clear();
	for (size_t i(0); i < level; ++i) {
		for (auto const & ctr : *_multiLevelConstraints[i]) {
			_instance.addMustLink(ctr.first, ctr.second);
		}
	}
	// construit les infos de correspondances entre les instances
	_instance.buildMustLink(aggregations);
	// construit l'instance aggrÃ©gÃ©e
	instance = KMInstance(_instance, aggregations);
}
// nbNodes : le nombre de noeuds minimal dans le graph de plus bas niveau
// critÃ¨re dÃ©fini pour ajouter les noeuds : voisin le plus proche
void MultiLevelAlgo::buildMultiLevelData(size_t nbNodes) {

	KMPartition partition(_instance, _instance.nbObs());
	// on crÃ©e les singletons
	for(size_t i(0); i<_instance.nbObs(); i++)
		partition.shift(i,i);

	while(partition.nbLabels() > nbNodes ){
		IndexedList used(partition.usedLabels());
		// dÃ©finit un nouveau niveau
		_multiLevelConstraints.push_back(new KMConstraints(_input.nbObs()));

		while(!used.empty()){
			size_t const m = used.pop_random();
			if( !used.empty()){
				// calculer la distance de ce centre avec les autres
				std::multimap<Double, size_t> neighbor;
				for(auto const & c : partition.usedLabels()){
					if ( m != c)
						neighbor.insert( std::make_pair(partition.getDistanceCenter(m,c) , c));
				}
				size_t const c(neighbor.begin()->second);
				_multiLevelConstraints.back()->newCtr(*partition.observations(m).begin(),*partition.observations(c).begin());
				partition.fusion(m,c);				
				// si plusieurs plusieurs plus pret : tirer au hazard (aprÃ©s)
				used.erase(c);
			}
		};
		// ajouter les contraintes associÃ©e Ã  ce niveau
	};
}
//
void MultiLevelAlgo::refine() {
	// lancer le KMEANS sur chaque niveau en partant du plus Ã©levÃ© (celui qui contient le moins de noeuds)
	// Ã  chaque fois on initialise avec le niveau prÃ©cÃ©dent (sauf le premier!)
	// Pour le premier faire un appel Ã  random(0);
	KMInstance instance;
	Aggregations aggregations;	
	Timer timer;
	// pour chaque level
	for (size_t level(0); level <= _multiLevelConstraints.size(); ++level) {
		// ! on parcours Ã  l'envers
		buildInstance(_multiLevelConstraints.size() - level, instance,aggregations);
		KMInput input(instance, _input.maxNbLabels());
		// initialiser cette input avec la solkution courante
		// attention il faut utiliser aggregation pour faire les neodus agrÃ©gÃ©s et la solution courante
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
