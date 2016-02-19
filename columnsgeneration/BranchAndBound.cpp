#include "BranchAndBound.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>
#include "LpMaster.hpp"
#include "Node.hpp"

#include "BinaryDecompositionOracle.hpp"
#include "MilpOracle.hpp"
#include "QpOracle.hpp"

BranchAndBound::BranchAndBound(ICliquePartitionProblem const &input,
		AvailableOracle oracle) :
		_input(&input), _master(NULL), _vnsGenerator(NULL), _mipGenerator(NULL), _decision() {
	_root = NULL;
	// maximisation
	_bestFeasible = -std::numeric_limits<double>::max();
	_bestPossible = +std::numeric_limits<double>::max();
	_current = NULL;
	_output = NULL;
	_master = new LpMaster(&input, &_decision);
	_mipGenerator = _input->newOracle(oracle, &_master->dual(), &_decision);
	_vnsGenerator = _input->newVnsOracle(&_master->dual(), &_decision);
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

	size_t const nColumnsByIte(0);

	if (_output != NULL) {
		output() << std::setw(6) << "ite";
		output() << std::setw(10) << "nb col";
		output() << std::setw(20) << "step";
		output() << std::setw(8) << "added";
		output() << std::setw(25) << "bd master";
		output() << std::setw(25) << "rd";
		output() << std::setw(15) << "time";
		output() << std::endl;
	}
	do {
		++ite;
		//write();
		timer.restart();
		//		_master->write();
		_master->solveMaster();
		m += timer.elapsed();
		timer.restart();
		//		bool heuristicSucceeded(false);
		_vnsGenerator->columns().clear();
		bool heuristicSucceeded(false);
		bool stopvns(false);
		for (size_t i(0); i < 5 && !stopvns; ++i) {
			if (nColumnsByIte == 0) {
				if (_vnsGenerator->run(1, true)) {
					heuristicSucceeded = true;
					stopvns = true;
				}
			} else {
				if (_vnsGenerator->run(1, false))
					heuristicSucceeded = true;
			}
		}
		//		if(!heuristicSucceeded )
		//			heuristicSucceeded  = _vnsGenerator->run(2, true)
		h += timer.elapsed();
		nb = 0;
		rd = -1;
		if (heuristicSucceeded) {
			step = "HEURISTIC";
			timer.restart();
			_vnsGenerator->sortedColumns(sorter);
			_master->add(sorter, nColumnsByIte, nb, rd);
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
			output() << std::setw(25) << std::setprecision(10)
					<< _master->obj();
			output() << std::setw(25) << std::setprecision(10) << rd;
			output() << std::setw(15) << std::setprecision(8)
					<< total.elapsed();
			output() << std::endl;
		}
	} while (!stop);
	std::cout << std::resetiosflags(std::ios::showbase);
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
	treat(_root);
	size_t ite(0);
	size_t noeud1;
	size_t noeud2;
	while (_nodesByUpperBounds.begin()->first > _bestFeasible + 1e-6) {
		_bestPossible = _nodesByUpperBounds.begin()->second->ub();
		++ite;
		Node * node = _nodesByUpperBounds.begin()->second;
		_nodesByUpperBounds.erase(_nodesByUpperBounds.begin());
		// on garde le noeud dans la pile pour la trace
		node->ub() = -1e50;
		_nodesByUpperBounds.insert(std::make_pair(node->ub(), node));

		_input->branchingSelection(*node, noeud1, noeud2);
		treat(new Node(node, true, noeud1, noeud2));
		treat(new Node(node, false, noeud1, noeud2));
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
	_input->writeSolution(_bestSolution, _bestPossible);
	//	std::ofstream file(
	//			GetStr("optimal/", _input->name(), "_", _bestPossible, ".txt").c_str());
	//	for (auto const & c : _bestSolution) {
	//		for (size_t r(0); r < _input->nR(); ++r) {
	//			for (size_t b(0); b < _input->nB(); ++b) {
	//				if (c.first->contains(r)
	//						&& c.first->contains(_input->nR() + b)) {
	//					file << std::setw(6) << 1 + r;
	//					file << std::setw(6) << 1 + b + _input->nR();
	//					file << std::endl;
	//				}
	//
	//			}
	//		}
	//	}
	//	file.close();
}
void BranchAndBound::setOutput(std::ostream & stream) {
	_output = &stream;

}
