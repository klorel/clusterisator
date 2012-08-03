/*
 * Shifter.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef SHIFTER_HPP_
#define SHIFTER_HPP_

#include "INeighborhood.hpp"

class Shifter: public INeighborhood {
public:
	Shifter(Data& data, ICriterion const & criterion, IndexedList const & nodes,
			IndexedList const & labels);
	virtual ~Shifter();
public:
	const_iterator begin() const;
	const_iterator end() const;

	size_t random() const;
	// recherche le premier movement améliorant, retourne true si un mouvement améliorant est déterminé
	bool findFirst(size_t const &);
	// recherche le meilleur mouvement améliorant, retourne true si un mouvement améliorant est déterminé
	bool findBest(size_t const &);
	// applique n'importe quel mouvement
	void findAny(size_t const &);
};

#endif /* SHIFTER_HPP_ */
