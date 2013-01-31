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
// @brief construit une suite de problèmes agrégés en agrégeant nbNodesMax noeuds par niveau et en produisant des graphes avec au plus nbNodes noeuds
// @param nbNodes    : limite pour le graph le plus agrégé 
// @param nbNodesMax : limite max de noeuds agrégé par étape
void MultiLevelAlgo::buildMultiLevelData(size_t nbNodes) {

	KMPartition partition(_instance, _instance.nbObs());
	// on crée les singletons
	for(size_t i(0); i<_instance.nbObs(); ++i)
		partition.shift(i,i);

	while(partition.nbLabels() > nbNodes ){
		IndexedList used(partition.usedLabels());
		// définit un nouveau niveau
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
				// si plusieurs plusieurs plus pret : tirer au hazard (aprés)
				used.erase(c);
			}
		};
		// ajouter les contraintes associée à ce niveau
	};
}
//
// _step: 
// _startLevel : niveau de départ pour le raffinement
// _startPoint : (attention doit être compatible avec le niveau de plus agrégé)
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
		HMeans<true>()(input);
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
// @brief lance l'algorithme multi-niveau à partir d'une suite d'agrégation, d'une partition de départ et d'un niveau de départ et avec un pas donné
// _step: 
// _startLevel : niveau de départ pour le raffinement
// _startPoint : (attention doit être compatible avec le niveau de plus agrégé)
void MultiLevelAlgo::launch(size_t nbNodes) {
	// initialisation au point de départ
	for (size_t i(0); i < _input.nbObs(); ++i) {
		_input.shiftForced(i, _startPoint.label(i));
	}		
	// lancement du run 
	refine();

}

void MultiLevelAlgo::getStartPoint( Partition & point){
	KMInstance instance;
	Aggregations aggregations;	
	buildInstance(_multiLevelConstraints.size(), instance,aggregations);
	KMInput input(instance, _input.maxNbLabels());
	input.random(0);	
	for (size_t i(0); i < _input.nbObs(); ++i) {
		point.shift(i, input.label(aggregations.newIds[i]));
	}	
}
