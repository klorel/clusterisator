/*
 * MultiLevelKMInstance.h
 *
 *  Created on: 15 dÃ©c. 2012
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
	MultiLevelAlgo(KMInstance const &, size_t k,Partition _startPoint=0);
	virtual ~MultiLevelAlgo();
public:
	// pour lancer les kmmeans
	void buildInstance(size_t level, KMInstance &, Aggregations &);
	//
	void launch();
	void refine();

public:
	// on agrège jusqu'�  ce que l'on ait nbNodes noeuds dans le problème équivalent
	// nbNodesMax = 0 pas de limite
	void buildMultiLevelData(size_t nbNodes, size_t nbNodesMax = 50);

	// initialisation
	void setStartLevel( size_t );
	void getStartPoint( Partition  &);
	void setStep(size_t );

	void setStartPoint( Partition  & );
	void setOut(std::ostream & =std::cout);
	//Double getDistance(SuperNoeud const & i, SuperNoeud const & j) const;

	std::ostream & out();
public:
	// l'instance de dÃ©part
	KMInstance _instance;
	KMInput _input;
	// les contraintes de chaque niveau : incrementalement
	std::vector<KMConstraints *> _multiLevelConstraints;

	size_t _startLevel;
	size_t _step;
	Partition _startPoint;
	std::ostream * _out;
	Number _totalKMtimer;
	size_t _totlKMite;

};

#endif /* MULTILEVELKMINSTANCE_H_ */