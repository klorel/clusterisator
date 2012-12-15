/*
* Input.cpp
*
*  Created on: 23 août 2012
*      Author: manuel
*/

#include "src/KMInput.hpp"

void KMInput::out(std::string const & name) const {
	OUT<< std::setw(10) << name;
	OUT << std::setw(10) <<std::setprecision(6)<< _timer.elapsed();
	OUT << std::setw(10) << _ite;
	OUT << std::setw(20) << std::setprecision(10) << cost();
	OUT << "\n";
}
void KMInput::headers() {
	OUT<< std::setw(10) << "ALGO";
	OUT<< std::setw(10) << "TIME";
	OUT<< std::setw(10) << "ITERATION";
	OUT<< std::setw(20) << "COST";
	OUT<< "\n";
}size_t & KMInput::ite() {
	return _ite;
}
size_t KMInput::ite() const {
	return _ite;
}

KMInput::KMInput(KMInstance const & instance, size_t k) :
	KMPartition(instance, k), _timer(), _ite(0), _modifiedLabels(
	maxNbLabels()), _modifiedObs(nbObs()) {
		_moves.reserve(nbObs());
		_buffer.reset(nbObs());
}

KMInput::KMInput(KMInstance const & instance, Partition const & p) :
	KMPartition(instance, p), _timer(), _ite(0), _modifiedLabels(
	maxNbLabels()), _modifiedObs(nbObs()) {
		_moves.reserve(nbObs());
		_buffer.reset(nbObs());
}

KMInput::~KMInput() {

}
void KMInput::shiftForced(size_t obs, size_t to) {
	//	std::cout << "KMInput\n";
	size_t const from(label(obs));
	KMPartition::shiftForced(obs, to);
	_modifiedLabels.insert(from);
	_modifiedLabels.insert(to);
	_modifiedObs.insert(obs);
}
bool KMInput::shift(size_t obs, size_t to) {
	//	std::cout << "KMInput\n";
	size_t const from(label(obs));
	if (KMPartition::shift(obs, to)) {
		_modifiedLabels.insert(from);
		_modifiedLabels.insert(to);
		_modifiedObs.insert(obs);
		return true;
	} else
		return false;
}
bool KMInput::shift(Move const & move) {
	size_t const from(label(move.first));
	if (KMPartition::shift(move)) {
		_modifiedLabels.insert(from);
		_modifiedLabels.insert(move.second);
		_modifiedObs.insert(move.first);
		return true;
	} else
		return false;

}
bool KMInput::applyMoves() {
	bool success(false);
	if (!_moves.empty()) {
		for (auto const & move : _moves) {
			if (shift(move))
				success = true;
		}
	}
	return success;
}
std::pair<size_t, Double> KMInput::getBest(size_t i) const {
	size_t const l(label(i));
	std::pair<size_t, Double> min(l, 0);

	if (sizeOfLabel(l) != 1) {
		Double const cst(-getDistance(i) * getCoeff<false>(i, l));
		for (size_t j(0); j < getK(); ++j) {
			if (j != l && feasible(i, j)) {
				Double delta(cst);
				delta += getDistance(i, j) * getCoeff<true>(i, j);
				delta *= obsWeight(i);
				if (delta < min.second) {
					min.first = j;
					min.second = delta;
				}
			}
		}
	}
	return min;
}
std::pair<size_t, Double> KMInput::getClosest(size_t i) const {
	size_t const l(label(i));
	std::pair<size_t, Double> min(l, d(i));
	//	for (size_t k(0); k < getK(); ++k) {
	//		if (k != l) {
	//			std::pair<size_t, Double> const d(k, getDistance(i, k));
	//			if (d.second < min.second) {
	//				min = d;
	//			}
	//		}
	//	}

	if (_modifiedLabels.contains(l)) {
		for (size_t k(0); k < getK(); ++k) {
			if (k != l) {
				std::pair<size_t, Double> const d(k, getDistance(i, k));
				if (d.second < min.second) {
					min = d;
				}
			}
		}
	} else {
		for (auto const & k : _modifiedLabels) {
			//	for (size_t k(0); k < getK(); ++k) {
			if (k != l) {
				std::pair<size_t, Double> const d(k, getDistance(i, k));
				if (d.second < min.second) {
					min = d;
				}
			}
		}
	}
	return min;
}
void KMInput::clearMoves() {
	_moves.clear();
}
void KMInput::clearModified() {
	_modifiedLabels.clear();
	_modifiedObs.clear();
}
Moves & KMInput::moves() {
	return _moves;
}
Moves const & KMInput::moves() const {
	return _moves;
}
void KMInput::fillModified() {
	_modifiedLabels.fill();
	_modifiedObs.fill();
}

// witch label should be tested for this observation
void KMInput::writeCandidates(size_t obs) {
	_buffer.clear();
	// SI PAS DE CONTRAINTES
	// -- si le label de obs a été modifié : tout les labels
	// -- si le label de obs n'a pas été modifié : seulement ceux qui ont été modifiés
	// SINON
	// -- si le label de obs a été modifié : tout les labels
	// -- si le label de obs n'a pas été modifié : seulement les labels des noeuds liés par des contraintes s'ils ont été modifiés

}
