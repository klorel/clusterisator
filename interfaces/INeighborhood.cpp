#include "../interfaces/INeighborhood.hpp"

void INeighborhood::build(Moves & moves, ISolution & solution) {
	moves.clear();
	for (size_t node(0); node < solution.partition().nbObs(); ++node) {
		Move move(node, &moves);
		compute(move, solution);
	}

}

// calcule le meilleur déplacement
void INeighborhood::compute(Move & move, ISolution & solution) {
	move.from = solution.partition().label(move.node);
	std::pair<Double, size_t> bestMove(0, move.from);
	for (auto const & label : solution.partition().used()) {
		if (label != move.from) {
			Double const delta = solution.criterion().getDeltaShift(solution,
					move.node, label);
			if (solution.criterion().isBetter(delta, bestMove.first)) {
				bestMove = std::make_pair(delta, label);
			}
		}
	}
	move.to = bestMove.second;
	move.container->insert(std::make_pair(bestMove.first, move));
}

void INeighborhood::apply(Move const & move, ISolution & solution,
		LinkedMoves & linkedMoves) {
	solution.shift(move.node, move.to);
	LinkedMoves::value_type list;
	list.merge(linkedMoves[move.from]);
	list.merge(linkedMoves[move.to]);
}

INeighborhood::~INeighborhood() {
}
