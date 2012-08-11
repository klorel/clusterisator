/*
 * LPA2.hpp
 *
 *  Created on: 22 juil. 2011
 *      Author: manuel
 */

#ifndef DATA_HPP_
#define DATA_HPP_

#include "common.h"

#include "IndexedList.hpp"
#include "Graph.hpp"
#include "IGraphPartition.hpp"
#include "Partition.hpp"

class GraphPartition: public IGraphPartition {
public:
public:
	GraphPartition(ILinks const &);
	virtual ~GraphPartition();
public:
	// déplacement d'un noeud
	void shift(size_t n, size_t p);
	// fusion de deux labels
	size_t fusion(size_t l1, size_t l2);
	// la taille d'un label
public:
	// le nombre de noeuds
	size_t nbObs() const;
	// le nombre d'arrêtes
	size_t nbLinks() const;
	// le nombre de labels utilisé dans la solution courante
	size_t nbLabels() const;
	// le nombre de noeud ayant la label l
	size_t & sizeOfLabel(size_t l);
	size_t sizeOfLabel(size_t l) const;
	// le degrés d'un label
	Double  degreeOfLabel(size_t l) const;
	Double & degreeOfLabel(size_t l);
	// récupère un label vide
	size_t getUnUsedLabel() const;
	// est-ce que tout les labels sont utilisés
	bool allLabelsUsed() const;
	// les labels libres
	IndexedList & unUsed();
	IndexedList const & unUsed() const;
	// les labels utilisés
	IndexedList & used();
	IndexedList const & used() const;
	// est-ce que le label est utilisé
	bool isUsed(size_t l) const;
public:
	IPartition const & nodeLabel() const;
	void set(IPartition const &);

	IndexedList const & usedLabel() const;
	IndexedList const & unUsedLabel() const;

	IntList & list(size_t label);
	IntList const & list(size_t label) const;

	size_t label(size_t n) const;
	size_t & label(size_t n);
	// can be overloaded in other implementations
	virtual Double intra(size_t label);
	virtual Double intra(size_t node, size_t label);
	void intra(size_t node, DoubleVector & values, bool const & reset);
public:
	Double computeDegreeOfLabel(size_t l) const;

	bool check() const;
	bool checkPosition() const;
	bool checkDegrees() const;

	bool checkDegree(size_t const l) const;
private:
	Partition _partition;
	// le degree de chaque label
	DoubleVector _labelDegree;

};
#endif /*DATA_HPP_ */
