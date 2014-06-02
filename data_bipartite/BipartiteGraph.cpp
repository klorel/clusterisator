#include "BipartiteGraph.hpp"

#include "BinaryDecompositionOracle.hpp"
#include "MilpOracle.hpp"
#include "QpOracle.hpp"
#include "Node.hpp"

BipartiteGraph::BipartiteGraph() :
		_m(0), _inv_m(0), _edges(), _a() {

}

BipartiteGraph::BipartiteGraph(Edges const & edges) :
		_m(0), _inv_m(0), _edges(edges), _a() {
	build();
}

BipartiteGraph::~BipartiteGraph() {
}
void BipartiteGraph::build() {
	size_t r(0);
	size_t b(0);
//	std::cout << _edges << std::endl;
	for (auto const & e : _edges) {
		r = std::max(r, e._i);
		b = std::max(b, e._j);
	}
	_a.allocate(r + 1, b + 1);
	_k.assign(r + 1 + b + 1, 0);
	for (auto const & e : _edges) {
		_a.set(e._i, e._j, e._v);
		_k[e._i] += e._v;
		_k[r + 1 + e._j] += e._v;
	}
	_m = (std::accumulate(_a.begin(), _a.end(), 0.0));
	_inv_m = 1.0 / _m;
	_allLinks.assign(nV(), std::map<size_t, double>());
	_costs.reserve(nR() + nB());
	for (size_t i(0); i < nR(); ++i) {
		for (size_t j(0); j < nB(); ++j) {
			double const value(w(i, j));
			if (value != 0) {
				_costs.push_back(Edge(i, nR() + j, value));
				_allLinks[i][nR() + j] = value;
				_allLinks[nR() + j][i] = value;
			}
		}
	}
}

//void BipartiteGraph::read(std::string const & fileName, std::ostream & stream) {
//	std::ifstream file(fileName.c_str());
//	//_a << file;
//	std::string line;
//	std::getline(file, line);
//	size_t r;
//	size_t b;
//	{
//		std::stringstream buffer(line);
//		buffer >> r;
//		buffer >> b;
//	}
//	_a.allocate(r, b);
//	while (std::getline(file, line)) {
//		std::stringstream buffer(line);
//		buffer >> r;
//		buffer >> b;
//		_a.get(r - 1, b - 1) = 1;
//	}
//	file.close();
//	build();
//}

Double BipartiteGraph::computeCost(std::set<size_t> const & v) const {
	Double result(0);
	for (auto const & r : v) {
		if (r < nR()) {
			for (auto const & b : v) {
				if (b >= nR()) {
					result += w(r, b - nR());
				}
			}
		}
	}
	return result;

}

std::string BipartiteGraph::name() const {
	return "";
}

IOracle * BipartiteGraph::newOracle(AvailableOracle oracle,
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
		result = new BinaryDecompositionOracle(this, dual, decision);
		break;
	}
	return result;
}
void BipartiteGraph::branchingSelection(Node const & node, size_t &noeud1,
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
std::pair<size_t, size_t> BipartiteGraph::branchingSelection(
		DecisionSet const & decisions, BranchingWeights & weights) const {
	BranchingWeights::const_iterator it(weights.begin());
	while (decisions.find(
			Decision(
					std::make_pair(it->second.first, nR() + it->second.second)))
			!= decisions.end() && it != weights.end()) {
		++it;
	}
	return std::make_pair(it->second.first, nR() + it->second.second);
}
void BipartiteGraph::branchingWeights(FractionnarySolution const & solution,
		BranchingWeights & weights) const {
// on cherche des arrêtes présentes et semi-présentes dans deux colonnes
	BranchingWeights2 temp;
	std::map<Edge, std::pair<IntSet, IntSet> > toto;
	for (auto const & kvp : solution) {
		//		std::cout << std::setw(6) << kvp.first->id();
		//		std::cout << std::setw(15) << kvp.second;
		//		std::cout << std::endl;
		for (Edge const & e : edges()) {
			bool const iFirst(kvp.first->contains(e._i));
			bool const iSecond(kvp.first->contains(e._j + nR()));
			if (iFirst && iSecond) {
				toto[e].first.insert(kvp.first->id());
			} else if (iFirst + iSecond == 1) {
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
		std::cout << "weights.empty(), generating full weights" << std::endl;
		for (auto const & kvp : solution) {
			for (size_t r(0); r < nR(); ++r) {
				for (size_t b(0); b < nB(); ++b) {
					bool const iR(kvp.first->contains(r));
					bool const iB(kvp.first->contains(b) + nR());
					if (iR && iB) {
						toto[Edge(r, b, 1)].first.insert(kvp.first->id());
					} else if (iR + iB == 1) {
						toto[Edge(r, b, 1)].second.insert(kvp.first->id());
					}
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
