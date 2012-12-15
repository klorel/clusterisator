/*
 * Input.hpp
 *
 *  Created on: 23 août 2012
 *      Author: manuel
 */

#ifndef INPUT_HPP_
#define INPUT_HPP_

#include "src/common.h"

#include "src/KMPartition.hpp"
#include "src/RectMatrix.hpp"
#include "src/Partition.hpp"
#include "src/Timer.hpp"

class KMInput: public KMPartition {
public:
	KMInput(KMInstance const &, size_t k = 1);
	KMInput(KMInstance const &, Partition const &);
	virtual ~KMInput();
public:
	void out(std::string const & name) const;
	void headers();
	size_t & ite();
	size_t ite() const;
	
	void shiftForced(size_t obs, size_t to);
	bool shift(size_t obs, size_t to);
	bool shift(Move const &);

	std::pair<size_t, Double> getClosest(size_t obs) const;
	std::pair<size_t, Double> getBest(size_t obs) const;

	void clearMoves();
	void clearModified();
	bool applyMoves();
	bool emptyMoves() const;
	Moves & moves();
	Moves const & moves() const;

	void fillModified();
private:
	void writeCandidates(size_t obs);
private:
	Timer _timer;
	size_t _ite;
	IndexedList _modifiedLabels;
	IndexedList _modifiedObs;
	Moves _moves;
	IndexedList _buffer;

};
inline bool KMInput::emptyMoves() const {
	return _moves.empty();
}

#endif /* INPUT_HPP_ */
