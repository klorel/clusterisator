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
#include <iostream>
#include <fstream>


MultiLevelAlgo::MultiLevelAlgo(KMInstance const & instance, size_t k,Partition startPoint) :
	_instance(instance), _input(_instance, k),_startPoint(startPoint) {
		// par defaut sur la sortie standard.
		setOut();
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


// @brief construit une suite de problÃ¨mes agrÃ©gÃ©s en agrÃ©geant nbNodesMax noeuds par niveau et en produisant des graphes avec au plus nbNodes noeuds
// @param nbNodes    : limite pour le graph le plus agrÃ©gÃ© 
// @param nbNodesMax : limite max de noeuds agrÃ©gÃ© par Ã©tape
void MultiLevelAlgo::buildMultiLevelData(size_t nbNodes,size_t nbNodesMax) {

	KMPartition partition(_instance, _instance.nbObs());
	// on crÃ©e les singletons
	for(size_t i(0); i<_instance.nbObs(); ++i)
		partition.shift(i,i);

	while(partition.nbLabels() > nbNodes ){
		std::cout << "partition.nbLabels() : "<<partition.nbLabels()<<std::endl;
		IndexedList used(partition.usedLabels());
		// definit un nouveau niveau
		_multiLevelConstraints.push_back(new KMConstraints(_input.nbObs()));
		//
		size_t compteur=0;
		while(!used.empty() && compteur < nbNodesMax){
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
				compteur++;
			}
		};
		// ajouter les contraintes associée �  ce niveau
	};
}
//
// _step: 
// _startLevel : niveau de dÃ©part pour le raffinement
// _startPoint : (attention doit Ãªtre compatible avec le niveau de plus agrÃ©gÃ©)
void MultiLevelAlgo::refine() {
	// lancer le KMEANS sur chaque niveau en partant du plus Ã©levÃ© (celui qui contient le moins de noeuds)
	// �  chaque fois on initialise avec le niveau précédent (sauf le premier!)
	// Pour le premier faire un appel �  random(0);
	KMInstance instance;
	Aggregations aggregations;	
	Timer timer;
	// pour chaque level
	size_t level;
	for ( level=_startLevel; level <= _multiLevelConstraints.size(); level+= _step) {
		// ! on parcours �  l'envers
		buildInstance(_multiLevelConstraints.size() - level, instance,aggregations);
		KMInput input(instance, _input.maxNbLabels());
		// initialiser cette input avec la solkution courante
		// attention il faut utiliser aggregation pour faire les neodus agrÃ©gÃ©s et la solution courante

		if(level!=_startLevel){	
			for (size_t i(0); i < _input.nbObs(); ++i) {
				input.shiftForced(aggregations.newIds[i], _input.label(i));
			}
		}

		// on lance l'algo
		HMeans<true>()(input);

		out()<<"Niveau de rafinement          : " <<_multiLevelConstraints.size() - level<<std::endl;
		out()<<"Nombre d'itération par etape : " <<input.ite()<<std::endl;
		out()<<"Temps ecoule                  : " <<timer.elapsed()<<std::endl;
		out()<<"Valeur du cout                : " <<input.cost()<<std::endl;
		out()<<std::endl;

		// suavegarde de la solution
		input.computeCenters();
		for (size_t i(0); i < _input.nbObs(); ++i) {
			_input.shiftForced(i, input.label(aggregations.newIds[i]));
		}
		for (size_t i(0); i < _input.nbObs(); ++i) {
			input.shiftForced(aggregations.newIds[i], _input.label(i));
		}
	}


	// pb ici traitement du dernier niveau


	/*if(level>_multiLevelConstraints.size())
	{
		// on lance l'algo
		HMeans<true>()(_input);
		out()<<"Niveau de rafinement         : " <<0<<std::endl;
		out()<<"Nombre d'iteration par etape : " <<_input.ite()<<std::endl;
		out()<<"Temps ecoule                 : " <<timer.elapsed()<<std::endl;
		out()<<"Valeur du cout               : " <<_input.cost()<<std::endl;
		out()<<std::endl;

	}*/
}
// @brief lance l'algorithme multi-niveau �  partir d'une suite d'agrégation, d'une partition de départ et d'un niveau de départ et avec un pas donné
// _step: 
// _startLevel : niveau de dÃ©part pour le raffinement
// _startPoint : (attention doit Ãªtre compatible avec le niveau de plus agrÃ©gÃ©)

void MultiLevelAlgo::setOut(std::ostream & stream){
	_out = &stream;
}
void MultiLevelAlgo::launch() {
	// initialisation au point de dÃ©part
	for (size_t i(0); i < _input.nbObs(); ++i) {
		_input.shiftForced(i, _startPoint.label(i));
	}		
	// lancement du run 
	refine();

}

void MultiLevelAlgo::getStartPoint( Partition  & point){
	KMInstance instance;
	Aggregations aggregations;	
	buildInstance(_multiLevelConstraints.size(), instance,aggregations);
	KMInput input(instance, _input.maxNbLabels());
	input.random(0);	
	for (size_t i(0); i < _input.nbObs(); ++i) {
		point.shift(i, input.label(aggregations.newIds[i]));
	}	
}


void MultiLevelAlgo::setStartLevel( size_t level){
	_startLevel=level;
}



void MultiLevelAlgo::setStep(size_t step){
	_step=step;
}


void MultiLevelAlgo::setStartPoint( Partition  & point){
	_startPoint = point;
}

std::ostream & MultiLevelAlgo::out(){
	return *_out;
}