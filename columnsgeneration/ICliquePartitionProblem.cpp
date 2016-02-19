#include "ICliquePartitionProblem.h"
#include "MilpOracle.hpp"
#include "QpOracle.hpp"
#include "VnsGenerator.hpp"
#include "Node.hpp"

IOracle * ICliquePartitionProblem::newOracle(AvailableOracle oracle,
		DoubleVector const * dual, DecisionList const * decision) const {
	IOracle * result(NULL);
	switch (oracle) {
	case MILP:
		result = new MilpOracle(this, dual, decision);
		break;
	case MIQP:
		result = new QpOracle(this, dual, decision);
		break;
	default:
		break;
	}
	return result;
}
IOracle * ICliquePartitionProblem::newVnsOracle(DoubleVector const * dual,
		DecisionList const * decision) const {
	return new VnsGenerator(this, dual, decision);
}

void ICliquePartitionProblem::branchingSelection(Node const & node, size_t &noeud1,
		size_t &noeud2) const {
	DecisionSet decisions;
	node.decisions(decisions);
	BranchingWeights weights;
	branchingWeights(node.lbSolution(), weights);
	std::pair<size_t, size_t> const noeud1noeud2(
			branchingSelection(decisions, weights));
	noeud1 = noeud1noeud2.first;
	noeud2 = noeud1noeud2.second;
}

std::pair<size_t, size_t> ICliquePartitionProblem::branchingSelection(
		DecisionSet const & decisions, BranchingWeights & weights) const {
	BranchingWeights::const_iterator it(weights.begin());
	while (decisions.find(
			Decision(std::make_pair(it->second.first, it->second.second)))
			!= decisions.end() && it != weights.end()) {
		++it;
	}
	return std::make_pair(it->second.first, it->second.second);
}
void ICliquePartitionProblem::branchingWeights(FractionnarySolution const & solution,
		BranchingWeights & weights) const {
// on cherche des arrêtes présentes et semi-présentes dans deux colonnes
	BranchingWeights2 temp;
	std::map<Edge, std::pair<IntSet, IntSet> > toto;
	for (auto const & kvp : solution) {
		//		std::cout << std::setw(6) << kvp.first->id();
		//		std::cout << std::setw(15) << kvp.second;
		//		std::cout << std::endl;
		for (Edge const & e : costs()) {
			bool const iFirst(kvp.first->contains(e._i));
			bool const iSecond(kvp.first->contains(e._j));
			if (iFirst && iSecond) {
				toto[e].first.insert(kvp.first->id());
			} else if (iFirst || iSecond) {
				toto[e].second.insert(kvp.first->id());
			}
		}
	}
//	std::cout << "synthese" << std::endl;
	weights.clear();
	for (auto const & t : toto) {
		//		std::cout << std::setw(6) << t.second.first.size();
		//		std::cout << std::setw(6) << t.second.second.size();
		//		std::cout << std::endl;
		if (!t.second.first.empty() && !t.second.second.empty()) {
			//			std::cout << std::setw(6) << t.first._i;
			//			std::cout << std::setw(6) << t.first._j;
			//			std::cout << std::endl;
			weights.insert(
					std::make_pair(
							0.5
									* ((int) ((t.second.first.size()
											+ t.second.second.size()))),
							std::make_pair(t.first._i, t.first._j)));
		}
	}
	if (weights.empty()) {
		std::cout << "Weights.empty(), generating full weights" << std::endl;
		for (auto const & kvp : solution) {
			for (Edge const & e : costs()) {
				bool const iR(kvp.first->contains(e._i));
				bool const iB(kvp.first->contains(e._j));
				if (iR && iB) {
					toto[Edge(e._i, e._j, 1)].first.insert(kvp.first->id());
				} else if (iR || iB) {
					toto[Edge(e._i, e._j, 1)].second.insert(kvp.first->id());
				}

			}
		}

		for (auto const & t : toto) {
			//		std::cout << std::setw(6) << t.second.first.size();
			//		std::cout << std::setw(6) << t.second.second.size();
			//		std::cout << std::endl;
			if (!t.second.first.empty() && !t.second.second.empty()) {
				//			std::cout << std::setw(6) << t.first._i;
				//			std::cout << std::setw(6) << t.first._j;
				//			std::cout << std::endl;
				weights.insert(
						std::make_pair(
								0.5
										* ((int) ((t.second.first.size()
												+ t.second.second.size()))),
								std::make_pair(t.first._i, t.first._j)));
			}
		}
		if (weights.empty())
			std::cout << "weights.empty()" << std::endl;
	}
}
