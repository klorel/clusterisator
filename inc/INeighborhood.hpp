/*
 * INeighborhood.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef I_NEIGHBORHOOD_HPP_
#define I_NEIGHBORHOOD_HPP_

#include "common.h"
#include "IndexedList.hpp"
class INeighborhood {
public:
	typedef IndexedList Container;
	typedef Container::iterator iterator;
	typedef Container::const_iterator const_iterator;
public:

	virtual const_iterator begin() const= 0;
	virtual const_iterator end() const= 0;

	virtual size_t random() const = 0;
	// recherche le premier movement améliorant, retourne true si un mouvement améliorant est déterminé
	virtual bool findFirst(size_t const &) = 0;
	// recherche le meilleur mouvement améliorant, retourne true si un mouvement améliorant est déterminé
	virtual bool findBest(size_t const &) = 0;
	// applique n'importe quel mouvement
	virtual void findAny(size_t const &) = 0;
public:
	Data & data();
	Data const & data() const;
	ICriterion const & criterion() const;
	IndexedList const & nodes() const;
	IndexedList const & labels() const;
public:
	INeighborhood(Data& data, ICriterion const & criterion,
			IndexedList const & nodes, IndexedList const & labels);
	virtual ~INeighborhood();
private:
	Data & _data;
	ICriterion const & _criterion;
	IndexedList const & _nodes;
	IndexedList const & _labels;

};

#endif /* INEIGHBORHOOD_HPP_ */
