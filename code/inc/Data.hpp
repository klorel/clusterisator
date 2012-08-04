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
#include "IExtendedPartition.hpp"

class Data: public IExtendedPartition {
public:
public:
	Data(IGraph const &);
	virtual ~Data();
public:
	// déplacement d'un noeud
	void shift(size_t const & n, size_t const & p);
	// fusion de deux labels
	size_t fusion(size_t const & l1, size_t const & l2);
	// la taille d'un label
public:
	// le nombre de noeuds
	size_t nbNodes() const;
	// le nombre d'arrêtes
	size_t nbEdges() const;
	// le nombre de labels utilisé dans la solution courante
	size_t nbLabels() const;
	// le nombre de noeud ayant la label l
	size_t & sizeOfLabel(size_t const & l);
	size_t const & sizeOfLabel(size_t const & l) const;
	// le degrés d'un label
	double const & degreeOfLabel(size_t const & l) const;
	double & degreeOfLabel(size_t const & l);
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
	bool isUsed(size_t const & l) const;
public:
	IntVector const & nodeLabel() const;
	void startWith(IntVector const &);

	IndexedList const & usedLabel() const;
	IndexedList const & unUsedLabel() const;

	IntList & list(size_t const & label);
	IntList const & list(size_t const & label) const;

	size_t const & label(size_t const & n) const;
	size_t & label(size_t const &n);

	IntVector sortLocation() const;

public:
	double computeDegreeOfLabel(size_t const & l) const;

	bool check() const;
	bool checkPosition() const;
	bool checkDecomposition() const;
	bool checkScore() const;
	bool checkDegrees() const;

	bool checkDegree(size_t const l) const;
private:
	// le degree de chaque label
	DoubleVector _labelDegree;

	// la composition de chaque label
	LabelLists _labelLists;

	// le label de chaque noeud
	IntVector _nodeLabel;
	// la position de chaque noeud dans les listes de composition
	NodePositions _nodePosition;
	// la taille des labels
	IntVector _size;
	// les labels utilisés
	IndexedList _usedLabel;
	// les labels libres
	IndexedList _unUsedLabel;

};
#endif /*DATA_HPP_ */
