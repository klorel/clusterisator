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
	MultiLevelAlgo(KMInstance const &, size_t k);
	virtual ~MultiLevelAlgo();
public:
	// on agrège jusqu'à ce que l'on ait nbNodes noeuds dans le problème équivalent
	void buildMultiLevelData(size_t nbNodes);
	// pour lancer les kmmeans
	void buildInstance(size_t level, KMInstance &, Aggregations &);
	//
	void launch(size_t nbNodes);
	void refine();
	//
	Double getDistance(size_t i, size_t j) const;
public:
	// l'instance de départ
	KMInstance _instance;
	KMInput _input;
	// les contraintes de chaque niveau : incrémentalement
	std::vector<KMConstraints *> _multiLevelConstraints;
};

#endif /* MULTILEVELKMINSTANCE_H_ */
