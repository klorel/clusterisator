#include "ClusteringProblem.h"
#include "Node.h"

ClusteringProblem::ClusteringProblem() {
	_exactOracle = NULL;
	_vnsOracle = NULL;
}
ClusteringProblem::~ClusteringProblem() {

}

void ClusteringProblem::setExactOracle(IOracle * oracle) {
	_exactOracle = oracle;
}
void ClusteringProblem::setVnsOracle(IOracle *oracle) {
	_vnsOracle = oracle;
}
IOracle * ClusteringProblem::getExactOracle() const {
	return _exactOracle;
}
IOracle * ClusteringProblem::getVnsOracle() const {
	return _vnsOracle;
}

void ClusteringProblem::branchingSelection(Node const & node, size_t &noeud1,
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

std::pair<size_t, size_t> ClusteringProblem::branchingSelection(
		DecisionSet const & decisions, BranchingWeights & weights) const {
	BranchingWeights::const_iterator it(weights.begin());
	while (decisions.find(
			Decision(std::make_pair(it->second.first, it->second.second)))
			!= decisions.end() && it != weights.end()) {
		++it;
	}
	return it->second;
}

bool ClusteringProblem::checkGradient(IndexedList const & nodes,
		DoubleVector const & g) const {
	bool result(true);
	DoubleVector values(nV(), 0);
	gradient(nodes, values);
	for (size_t v(0); v < nV(); ++v) {
		if (std::fabs(g[v] - values[v]) > 1e-10) {
			result = false;
			std::cout << "WRONG GRADIENT " << std::setw(6) << v;
			std::cout << std::setw(16) << g[v];
			std::cout << std::setw(16) << values[v];
			std::cout << std::endl;
		}
	}
	return result;
}


std::string ClusteringProblem::problemName() const {
	return "";
}
