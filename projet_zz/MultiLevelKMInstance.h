/*
 * MultiLevelKMInstance.h
 *
 *  Created on: 15 dÃƒÂ©c. 2012
 *      Author: manuel
 */

#ifndef MULTILEVELKMINSTANCE_H_
#define MULTILEVELKMINSTANCE_H_

#include "../src/common.h"
#include "../src/KMInstance.hpp"
#include "../src/KMInput.hpp"


class MultiLevelAlgoStat{
public:
	size_t _level;
	size_t _ite;
	double _time;
	double _cost;
	MultiLevelAlgoStat(){}
	MultiLevelAlgoStat(size_t l, size_t i, double t, double c)
		:_level(l)
		,_ite(i)
		,_time(t)
		,_cost(c){
	}

};

typedef std::map<size_t, MultiLevelAlgoStat>  MultiLevelAlgoStats;
class MultiLevelAlgo {
public:
public:
	MultiLevelAlgo(KMInstance const &, size_t k);
	virtual ~MultiLevelAlgo();
public:
	// pour lancer les kmmeans
	void buildInstance(size_t level, KMInstance &, Aggregations &);
	//
	void launch();
	void refine();
	size_t nbLevels() const ;
public:
	// on agrÃ¨ge jusqu'Ã  ce que l'on ait nbNodes noeuds dans le problÃ¨me Ã©quivalent
	// nbNodesMax = 0 pas de limite
	void buildMultiLevelData(double nbNodes, double nbNodesMax = 50);
	void buildMultiLevelData_seuil(double nbNodes, double nbNodesMax = 50);
	void buildMultiLevelData_tab(double nbNodes);

	// initialisation
	void setStartLevel( size_t );
	void getStartPoint( Partition  &);
	void setStep(size_t );
	size_t getNbRejet();

	void setStartPoint( Partition  & );
	void setOut(std::ostream & =std::cout);
	void setSeuil(double);
	double getSeuil();
	//Double getDistance(SuperNoeud const & i, SuperNoeud const & j) const;
	MultiLevelAlgoStats const & stats()const;
	std::ostream & out();
public:
	// l'instance de dÃƒÂ©part
	KMInstance _instance;
	KMInput _input;
	// les contraintes de chaque niveau : incrementalement
	std::vector<KMConstraints *> _multiLevelConstraints;

	int _startLevel;
	int _step;
	Partition _startPoint;
	std::ostream * _out;
	Number _totalKMtimer;
	size_t _totlKMite;
	MultiLevelAlgoStats _stats;
	double _seuil;
	size_t _nbRejet;

};

#endif /* MULTILEVELKMINSTANCE_H_ */
