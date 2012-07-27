/*
 * Shifter.cpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#include "Shifter.hpp"
#include "IndexedList.hpp"

Shifter::Shifter(Data& data, ICriterion const & criterion,
		IndexedList const & nodes, IndexedList const & labels) :
		INeighborhood(data, criterion, nodes, labels) {

}

Shifter::~Shifter() {

}
Shifter::const_iterator Shifter::begin() const {
	return nodes().begin();
}

Shifter::const_iterator Shifter::end() const {
	return nodes().end();
}

size_t Shifter::random() const {
	return nodes().random();
}
// recherche le premier movement améliorant, retourne true si un mouvement améliorant est déterminé
bool Shifter::findFirst(size_t const & node) {
	bool improvement(false);
	return improvement;
}
// recherche le meilleur mouvement améliorant, retourne true si un mouvement améliorant est déterminé
bool Shifter::findBest(size_t const & node) {
	bool improvement(false);
	return improvement;
}
// applique n'importe quel mouvement
void Shifter::findAny(size_t const & node) {

}
