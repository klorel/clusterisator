/*
* MultiLevelKMInstance.cpp
*
*  Created on: 15 dÃƒÂ©c. 2012
*      Author: manuel
*/

#include "../projet_zz/MultiLevelKMInstance.h"
#include "../src/HMeans.hpp"
#include "../src/KMAlgo.hpp"
#include "../src/Timer.hpp"
#include <iostream>
#include <fstream>


MultiLevelAlgo::MultiLevelAlgo(KMInstance const & instance, size_t k) :
	_instance(instance), _input(_instance, k),_startPoint(_instance.nbObs(), k) {
		// par defaut sur la sortie standard.
		setOut();
		_step =1;
}



MultiLevelAlgo::~MultiLevelAlgo() {
	for (auto & ptr : _multiLevelConstraints)
		delete ptr;
}
void MultiLevelAlgo::buildInstance(size_t level, KMInstance & instance,Aggregations & aggregations) {
	_instance.mustLinks().clear();
	_instance.cannotLinks().clear();
	for (size_t i(0); i < level; ++i) {
		for (auto const & ctr : *_multiLevelConstraints[i]) {
			_instance.addMustLink(ctr.first, ctr.second);
		}
	}
	_instance.buildMustLink(aggregations);
	instance = KMInstance(_instance, aggregations);
}


// @brief construit une suite de problÃƒÂ¨mes agrÃƒÂ©gÃƒÂ©s en agrÃƒÂ©geant nbNodesMax noeuds par niveau et en produisant des graphes avec au plus nbNodes noeuds
// @param nbNodes    : limite pour le graph le plus agrÃƒÂ©gÃƒÂ© 
// @param nbNodesMax : limite max de noeuds agrÃƒÂ©gÃƒÂ© par ÃƒÂ©tape
void MultiLevelAlgo::buildMultiLevelData(double nbNodes,double nbNodesMax) {
	#if 1
	KMPartition partition(_instance, _instance.nbObs());
	// on crÃƒÂ©e les singletons
	for(size_t i(0); i<_instance.nbObs(); ++i)
		partition.shift(i,i);

	while(partition.nbLabels() > nbNodes ){
		std::cout << "partition.nbLabels() : "<<partition.nbLabels()<<std::endl;
		IndexedList used(partition.usedLabels());
		// definit un nouveau niveau
		_multiLevelConstraints.push_back(new KMConstraints(_input.nbObs()));
		//
		size_t compteur(0);
		while(!used.empty() && compteur < nbNodesMax && partition.nbLabels() > nbNodes ){
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
				// si plusieurs plusieurs plus pret : tirer au hazard (aprÃƒÂ©s)
				used.erase(c);
				compteur++;
			}
		};
		// ajouter les contraintes associÃ©e Ã  ce niveau
	};
	std::cout << "nbNodes     "<<partition.nbLabels() <<std::endl;
	std::cout << "nbNodesMax  "<<nbNodesMax <<std::endl;
	std::cout << "Built       "<<nbLevels() << " aggregation levels"<<std::endl;
	#else
		_multiLevelConstraints.push_back(new KMConstraints(_input.nbObs()));
		_multiLevelConstraints.back()->newCtr(0,2);
		_multiLevelConstraints.back()->newCtr(1,2);
	#endif
}
//
// _step: 
// _startLevel : niveau de dÃƒÂ©part pour le raffinement
// _startPoint : (attention doit ÃƒÂªtre compatible avec le niveau de plus agrÃƒÂ©gÃƒÂ©)
void MultiLevelAlgo::refine() {
	// lancer le KMEANS sur chaque niveau en partant du plus ÃƒÂ©levÃƒÂ© (celui qui contient le moins de noeuds)
	// Ã  chaque fois on initialise avec le niveau prÃ©cÃ©dent (sauf le premier!)
	// Pour le premier faire un appel Ã  random(0);
	KMInstance instance;
	Aggregations aggregations;	
	Timer timer;
	//std::cout << "_input value : "<<_input.computeCost() << std::endl;
	// pour chaque level
	for ( int level(_startLevel); level >= 0; level-= _step) {
	//for ( size_t level(_startLevel); level <= _multiLevelConstraints.size(); level+= _step) {
		// ! on parcours Ã  l'envers
		buildInstance( level, instance,aggregations);
		//for (size_t i(0); i < aggregations.v.size(); ++i) {		
		//	assert(!aggregations.v[i].empty());
		//	size_t const l(_input.label(*aggregations.v[i].begin()));
		//	
		//	for(auto const & j : aggregations.v[i]){
		//		assert(aggregations.newIds[j] == i);
		//		assert( _input.label(j) == l);
		//	}
		//}
		timer.restart();
		KMInput input(instance, _input.maxNbLabels());
		//std::cout << input.centers() << std::endl;
		//input.computeCenters();
		//for (size_t i(0); i < _input.nbObs(); ++i) {
		// il faut une solution faisable 
		for (size_t i(0); i < aggregations.v.size(); ++i) {			
			input.shiftForced(i, _input.label(*aggregations.v[i].begin() ));			
		}		
		//std::cout << input.centers() << std::endl;
		//input.computeCenters();
		//std::cout << input.centers() << std::endl;
		//std::cout << " input value : "<< input.computeCost() << std::endl;
		// on lance l'algo
		HMeans<false>()(input);
		// sauvegarde de la solution
		//for (size_t i(0); i < _input.nbObs(); ++i) {
		//	_input.shiftForced(i, input.label(aggregations.newIds[i]));
		//}
		for (size_t i(0); i < aggregations.v.size(); ++i) {
			for(auto const & j : aggregations.v[i])
				_input.shiftForced(j, input.label(i) );			
		}
		//for(auto const & i : aggregations.v){
		//	assert(!i.empty());
		//	size_t const l(_input.label(*i.begin()));
		//	for(auto const & j : i)
		//		assert( _input.label(j) == l);
		//}
		//std::cout << "_input value : "<<_input.computeCost() << std::endl;
		// on stocke les stats
		_stats[ level] = MultiLevelAlgoStat( level, input.ite(), timer.elapsed(), input.cost());
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
// @brief lance l'algorithme multi-niveau Ã  partir d'une suite d'agrÃ©gation, d'une partition de dÃ©part et d'un niveau de dÃ©part et avec un pas donnÃ©
// _step: 
// _startLevel : niveau de dÃƒÂ©part pour le raffinement
// _startPoint : (attention doit ÃƒÂªtre compatible avec le niveau de plus agrÃƒÂ©gÃƒÂ©)

void MultiLevelAlgo::setOut(std::ostream & stream){
	_out = &stream;
}
void MultiLevelAlgo::launch() {
	_stats.clear();
	std::cout << "Step  : "<<_step		<< std::endl;
	std::cout << "Level : "<<_startLevel<< std::endl;
	// initialisation au point de depart
	for (size_t i(0); i < _input.nbObs(); ++i) {
		_input.shiftForced(i, _startPoint.label(i));
		//file << i << " - "<< _startPoint.label(i)<<std::endl;
	}		
	//KMInstance instance;
	//Aggregations aggregations;	
	//for ( int level(0); level <= nbLevels(); level+= _step) {
	//	std::cout << "checking " << level << std::endl;
	//	buildInstance(level, instance,aggregations);
	//	KMInput input(instance, _input.maxNbLabels());
	//	std::cout << input.centers() << std::endl;
	//	std::ofstream file(std::to_string(level).c_str());
	//	for(auto const & i : aggregations.v){
	//		if(i.size()>1)
	//		DisplayContainer(file , i);
	//	}
	//	file.close();
	//	for(auto const & i : aggregations.v){
	//		assert(!i.empty());
	//		size_t const l(_input.label(*i.begin()));
	//		DisplayContainer(std::cout , i);
	//		for(auto const & j : i)
	//			assert( _input.label(j) == l);
	//	}
	//}
	std::cout << "Starting point value : "<<_input.computeCost() << std::endl;

	// lancement du run 
	refine();

}
size_t MultiLevelAlgo::nbLevels()const{
	return _multiLevelConstraints.size();
}
void MultiLevelAlgo::getStartPoint( Partition  & point){
	KMInstance instance;
	Aggregations aggregations;	
	buildInstance(nbLevels(), instance,aggregations);
	KMInput input(instance, _input.maxNbLabels());
	input.random(0);	
	std::cout << "Starting point value : "<<input.computeCost() << std::endl;
	for (size_t i(0); i < _input.nbObs(); ++i) {
		point.shift(i, input.label(aggregations.newIds[i]));
	}
	//KMInput input2(_instance, _input.maxNbLabels());
	//// initialiser cette input avec la solkution courante
	//// attention il faut utiliser aggregation pour faire les neodus agrÃƒÂ©gÃƒÂ©s et la solution courante
	//for (size_t i(0); i < _input.nbObs(); ++i) {
	//	input2.shiftForced(i, point.label(i));
	//}
	//std::cout << "Starting point value : "<<input2.computeCost() << std::endl;
	//for ( int level(nbLevels()); level >= 0; level-= _step) {
	//	buildInstance(level, instance,aggregations);
	//	for(auto const & i : aggregations.v){
	//		assert(!i.empty());
	//		size_t const l(_input.label(*i.begin()));
	//		for(auto const & j : i)
	//			assert( _input.label(j) == l);
	//	}
	//}
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
MultiLevelAlgoStats const & MultiLevelAlgo::stats()const{
	return _stats;
}

