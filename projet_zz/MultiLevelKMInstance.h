/*
 * MultiLevelKMInstance.h
 *
 *  Created on: 15 déc. 2012
 *      Author: manuel
 */

#ifndef MULTILEVELKMINSTANCE_H_
#define MULTILEVELKMINSTANCE_H_

#include "../src/common.h"
#include "../src/KMInstance.hpp"
#include "../src/KMInput.hpp"

class MultiLevelAlgo {
public:
public:
	MultiLevelAlgo(KMInstance const &, size_t k);
	virtual ~MultiLevelAlgo();
public:
	// pour lancer les kmmeans
	void buildInstance(size_t level, KMInstance &, Aggregations &);
	//
	void launch(size_t nbNodes);
	void refine();

public:
	// on agrège jusqu'à ce que l'on ait nbNodes noeuds dans le problème équivalent
	// nbNodesMax = 0 pas de limite
	void buildMultiLevelData(size_t nbNodes, size_t nbNodesMax = 0);

	// initialisation
	void setStartLevel( size_t );
	void setStartPoint( Partition const &);
	void setStep(size_t );

	void getStartPoint( Partition & );
	
	//Double getDistance(SuperNoeud const & i, SuperNoeud const & j) const;
public:
	// l'instance de départ
	KMInstance _instance;
	KMInput _input;
	// les contraintes de chaque niveau : incrémentalement
	std::vector<KMConstraints *> _multiLevelConstraints;

	size_t _startLevel;
	size_t _step;
	Partition _startPoint;

	Number _totalKMtimer;
	size_t _totlKMite;

};

#endif /* MULTILEVELKMINSTANCE_H_ */
