/*
 * INeighborhood.hpp
 *
 *  Created on: 27 juil. 2012
 *      Author: manuel
 */

#ifndef I_NEIGHBORHOOD_HPP_
#define I_NEIGHBORHOOD_HPP_

#include "../utils/common.h"
#include "../utils/IndexedList.hpp"
#include "../interfaces/ISolution.hpp"
#include "../interfaces/ICriterion.hpp"

class INeighborhood {
public:
	// admettons que l'on fasse du déplacement bete et méchant :)
	struct Move;
	struct MoveCompare;
	typedef std::map<Double, Move, MoveCompare> Moves;
	typedef std::vector<std::list<Moves::iterator> > LinkedMoves;

	struct Move {
		size_t const node;
		size_t from;
		size_t to;

		Moves * container;
		Move(size_t node, Moves * rhs) :
				node(node), from(node), to(node), container(rhs) {

		}
	};

	struct MoveCompare {
		MoveCompare(ICriterion const & criterion) :
				criterion(criterion) {
		}
		ICriterion const & criterion;

		bool operator()(Double const & a, Double const & b) const {
			return criterion.isBetter(a, b);
		}
	};

public:
	void build(Moves &, ISolution &);
	// calcule le meilleur déplacement
	void compute(Move & move, ISolution & solution);

	void apply(Move const & move, ISolution & solution, LinkedMoves  &);

	virtual ~INeighborhood();
};

#endif /* INEIGHBORHOOD_HPP_ */
