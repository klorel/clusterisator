#include "BranchAndBound.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>
#include "LpMaster.hpp"
#include "Node.hpp"

BranchAndBound::BranchAndBound(BipartiteGraph const &input) :
		_input(&input), _decision(), _master(&input, &_decision), _vnsGenerator(
				&input, &_master.dual(), &_decision), _mipGenerator(&input,
				&_master.dual(), &_decision) {
	_root = NULL;
	_rd = 0;
	_bestFeasible = -1e50;
	_bestPossible = +1e50;
	_current = NULL;
	_output = NULL;
}
BranchAndBound::BranchAndBound(BipartiteGraph const *input) :
		_input(input), _decision(), _master(input, &_decision), _vnsGenerator(
				input, &_master.dual(), &_decision), _mipGenerator(input,
				&_master.dual(), &_decision) {
	_root = NULL;
	_rd = 0;
	_bestFeasible = -1e50;
	_bestPossible = +1e50;
	_current = NULL;
	_output = NULL;
}

BranchAndBound::~BranchAndBound() {
	if (_root != NULL)
		delete _root;
}

void BranchAndBound::columnGeneration() {
	bool stop(false);

	//_master.readColumns("bug.txt");
	Timer timer;
	Double m(0);
	Double h(0);
	Double e(0);
	Timer total;
	std::multimap<Double, Column const *, std::greater<Double> > sorter;
	std::string step;
	size_t nb(0);
	size_t ite(0);

	do {
		++ite;
		//write();
		timer.restart();
		_master.solveMaster();
		m += timer.elapsed();
		timer.restart();
//		bool heuristicSucceeded(false);
		bool heuristicSucceeded(_vnsGenerator.run());
		h += timer.elapsed();

		if (heuristicSucceeded) {
			step = "HEURISTIC";
			_rd = -1;
			size_t p(0);
			_vnsGenerator.sortedColumns(sorter);
			nb = 0;
			for (auto const & column : sorter) {
//				std::cout << "\t"<<column
				_master.add(*column.second);
				_rd = std::max(_rd, column.first);
				++p;
				++nb;
				if (p >= 10)
					break;
			}

			//for(auto const & column : _vnsGenerator.columns()){
			//	_master.add(column);
			//	_rd = std::max(_rd, column.reducedCost() );
			//	++p;
			//	if(p>100)
			//		break;
			//}
		} else {
			step = "EXACT";
			nb = 0;
			timer.restart();
			//stop = !generate();
			stop = !_mipGenerator.generate();
			e += timer.elapsed();
			if (!stop) {
				_rd = -1;
				for (auto const & column : _mipGenerator.columns()) {
					++nb;
					_master.add(column);
					_rd = std::max(_rd, column.reducedCost());
				}
			} else
				_rd = _mipGenerator.bestReducedCost();
		}
		if (_output != NULL) {
			output() << std::setw(6) << ite;
			output() << std::setw(10) << _master.columns().size();
			output() << std::setw(20) << step;
			output() << std::setw(10) << nb;
			output() << std::setw(20) << std::setprecision(10) << _master.obj();
			output() << std::setw(20) << std::setprecision(10) << _rd;
			output() << std::setw(20) << total.elapsed();
			output() << std::endl;
		}
	} while (!stop);

//	_master.writeColumns("restart.txt");
}

void BranchAndBound::branch(Node * node, size_t r, size_t b) {
	// on cr�e un nouveau nouveau noeud,
	// on ajoute la contrainte
	// on ajoute le noeud � la pile avec la ub � recalculer ?
}

void BranchAndBound::init() {
	if (_root != NULL)
		delete _root;
	_root = new Node(*this);
//	_master.readColumns("treat.txt");
//	_master.readColumns("restart.txt");
//	_master.solveMaster();
//	_master.write("restart.lp");
	_master.addSingleton();
	_master.addEdge();
}
void BranchAndBound::run() {
	BranchingWeights weights;
	DecisionSet decisions;

	treat(_root);
	size_t ite(0);
	while (_nodesByUpperBounds.begin()->first > _bestFeasible + 1e-6) {
		_bestPossible = _nodesByUpperBounds.begin()->second->ub();
		++ite;
		Node * node = _nodesByUpperBounds.begin()->second;
		_nodesByUpperBounds.erase(_nodesByUpperBounds.begin());
		// on garde le noeud dans la pile pour la trace
		node->ub() = -1e50;
		_nodesByUpperBounds.insert(std::make_pair(node->ub(), node));

		_master.branchingWeights(node->lbSolution(), weights);
		BranchingWeights::const_iterator it(weights.begin());

		decisions.clear();
		node->decisions(decisions);
		while (decisions.find(Decision(it->second)) != decisions.end()
				&& it != weights.end()) {
			++it;
		}
		size_t const r(it->second.first);
		size_t const b(it->second.second);
		treat(new Node(node, true, r, b));
		treat(new Node(node, false, r, b));
		assert(
				_bestPossible
						>= _nodesByUpperBounds.begin()->second->ub() - 1e-6);

	}
	printTree();
}

void BranchAndBound::treat(Node * node) {
	_current = node;
	_current->id() = _nodesByUpperBounds.size();
	std::ofstream file(
			GetStr("node/", _input->name(), "_node_", _current->id()).c_str());
	_output = &file;
	_decision.clear();
	_current->decisions(_decision);
	_master.applyBranchingRule();
//	_master.write();
	_mipGenerator.applyBranchingRule();
//	_mipGenerator.write();
	columnGeneration();
//	_master.writeColumns("treat.txt");
	_current->ub() = _master.obj();
	_current->isInteger() = _master.getSolution(_current->lbSolution());
	if (_current->isInteger()) {
		_current->lb() = _master.obj();
		if (_bestFeasible < _current->lb()) {
			_bestFeasible = _current->lb();
			_solution = _current->lbSolution();
			writeSolution();
		}
		std::cout << "integer solution found, obj is " << _current->lb()
				<< std::endl;
	}
	if (_output != NULL) {
		for (auto const & i : _current->lbSolution()) {
			output() << std::setw(10) << i.second << std::endl;
		}
	}
	_nodesByUpperBounds.insert(std::make_pair(_current->ub(), _current));
	file.close();
	_output = NULL;
}
std::ostream & BranchAndBound::output() {
	return *_output;
}

void BranchAndBound::printTree(std::ostream &stream) const {
	stream << "printing tree of size " << _nodesByUpperBounds.size()
			<< std::endl;
	DecisionList list;
	for (auto const & node : _nodesByUpperBounds) {
		if (node.first != -1e50) {
			stream << std::setw(10) << node.second->id();
//		stream << std::setw(25) << node.first;
			stream << std::setw(25) << node.second->ub();
			stream << std::setw(25) << node.second->lb();
			stream << std::setw(3) << node.second->isInteger();
//			stream << std::setw(3) << node.second->decision().cannot();
//			stream << std::setw(3) << node.second->decision().r();
//			stream << std::setw(3) << node.second->decision().b();
			stream << std::endl;
		}
	}
	stream << "----------------------------" << std::endl;
}

LpMaster & BranchAndBound::master() {
	return _master;
}
LpMaster const & BranchAndBound::master() const {
	return _master;
}
void BranchAndBound::writeSolution() const {
	std::ofstream file(
			GetStr("optimal/", _input->name(), "_", _current->lb(), ".txt").c_str());
	for (auto const & c : _solution) {
		for (auto const & r : c.first->r())
			for (auto const & b : c.first->b()) {
				file << std::setw(6) << 1 + r;
				file << std::setw(6) << 1 + b + _input->nR();
				file << std::endl;
			}
	}
	file.close();
}
