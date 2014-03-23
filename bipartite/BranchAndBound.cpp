#include "BranchAndBound.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>
#include "LpMaster.hpp"
#include "Node.hpp"

BranchAndBound::BranchAndBound(BipartiteGraph const &input) :
		_input(&input), _master(NULL), _vnsGenerator(NULL), _mipGenerator(NULL), _decision() {
	_root = NULL;
	_bestFeasible = std::numeric_limits<double>::min();
	_bestPossible = std::numeric_limits<double>::max();
	_current = NULL;
	_output = NULL;
	_master = new LpMaster(&input, &_decision);
	_mipGenerator = new MipGenerator(&input, &_master->dual(), &_decision);
	_vnsGenerator = new VnsGenerator(&input, &_master->dual(), &_decision);
}

BranchAndBound::~BranchAndBound() {
	if (_root != NULL)
		delete _root;
	if (_master != NULL)
		delete _master;
	if (_mipGenerator != NULL)
		delete _mipGenerator;
	if (_vnsGenerator != NULL)
		delete _vnsGenerator;
}

void BranchAndBound::columnGeneration() {
	bool stop(false);

	//_master->readColumns("bug.txt");
	Timer timer;
	Double m(0);
	Double h(0);
	Double e(0);
	Double a(0);
	Timer total;
	ReducedCostSorter sorter;
	std::string step;
	size_t nb(0);
	size_t ite(0);
	Double rd(0);

	output() << std::setw(6) << "ite";
	output() << std::setw(10) << "nb col";
	output() << std::setw(20) << "step";
	output() << std::setw(8) << "added";
	output() << std::setw(20) << "lb master";
	output() << std::setw(20) << "rd";
	output() << std::setw(10) << "time";
	output() << std::endl;
	do {
		++ite;
		//write();
		timer.restart();
		_master->solveMaster();
//		_master->write();
		m += timer.elapsed();
		timer.restart();
//		bool heuristicSucceeded(false);
		bool heuristicSucceeded(false);
		heuristicSucceeded = _vnsGenerator->run(100, false);
		h += timer.elapsed();
		nb = 0;
		rd = -1;
		if (heuristicSucceeded) {
			step = "HEURISTIC";
			timer.restart();
			_vnsGenerator->sortedColumns(sorter);
			_master->add(sorter, 1, nb, rd);
			a += timer.elapsed();
		} else {
			step = "EXACT";
			timer.restart();
			//stop = !generate();
			stop = !_mipGenerator->generate();
			e += timer.elapsed();
			if (!stop) {
				timer.restart();
				_master->add(_mipGenerator->columns(), nb, rd);
				a += timer.elapsed();
			} else
				rd = _mipGenerator->bestReducedCost();
		}
		if (_output != NULL) {
			output() << std::setw(6) << ite;
			output() << std::setw(10) << _master->columns().size();
			output() << std::setw(20) << step;
			output() << std::setw(8) << nb;
			output() << std::setw(20) << std::setprecision(10)
					<< _master->obj();
			output() << std::setw(20) << std::setprecision(10) << rd;
			output() << std::setw(10) << std::setprecision(4)
					<< total.elapsed();
			output() << std::endl;
		}
	} while (!stop);

}

void BranchAndBound::init() {
	if (_root != NULL)
		delete _root;
	_root = new Node(*this);
//	_master->readColumns("treat.txt");
//	_master->readColumns("restart.txt");
//	_master->solveMaster();
//	_master->write("restart.lp");
	//_master->addSingleton();
	//_master->addEdge();
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

		_master->branchingWeights(node->lbSolution(), weights);
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
//	std::ofstream file(
//			GetStr("node/", _input->name(), "_node_", _current->id()).c_str());

	_decision.clear();
	_current->decisions(_decision);
	_master->applyBranchingRule();

	_mipGenerator->applyBranchingRule();

	columnGeneration();

	_current->ub() = _master->obj();
	_current->isInteger() = _master->getSolution(_current->lbSolution());
	if (_current->isInteger()) {
		_current->lb() = _master->obj();
		if (_bestFeasible < _current->lb()) {
			_bestFeasible = _current->lb();
			_bestSolution = _current->lbSolution();
			writeSolution();
		}
		std::cout << "integer solution found, obj is " << std::setprecision(10)
				<< _current->lb() << std::endl;
	}
	if (_output != NULL) {
		for (auto const & i : _current->lbSolution()) {
			output() << std::setw(10) << i.second << std::endl;
		}
	}
	_nodesByUpperBounds.insert(std::make_pair(_current->ub(), _current));
//	file.close();
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

IMaster & BranchAndBound::master() {
	return *_master;
}
IMaster const & BranchAndBound::master() const {
	return *_master;
}
void BranchAndBound::writeSolution() const {
	std::ofstream file(
			GetStr("optimal/", _input->name(), "_", _current->lb(), ".txt").c_str());
	for (auto const & c : _bestSolution) {
		for (auto const & r : c.first->r())
			for (auto const & b : c.first->b()) {
				file << std::setw(6) << 1 + r;
				file << std::setw(6) << 1 + b + _input->nR();
				file << std::endl;
			}
	}
	file.close();
}
void BranchAndBound::setOutput(std::ostream & stream) {
	_output = &stream;

}
