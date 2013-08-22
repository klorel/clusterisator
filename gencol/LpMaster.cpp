#include "LpMaster.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"

#include <cplex.h>

LpMaster::LpMaster(BipartiteGraph const &input, DecisionList const & decisions) :
		_input(&input), _env(NULL), _lp(NULL), _decisions(&decisions) {
	build();
}
LpMaster::LpMaster(BipartiteGraph const *input, DecisionList const * decisions) :
		_input(input), _env(NULL), _lp(NULL), _decisions(decisions) {
	build();
}

void LpMaster::build() {
	int err;
	_env = CPXopenCPLEX(&err);
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	CPXsetintparam(_env, CPX_PARAM_THREADS, 1);

	initLp();
//	_rAndbInColumn.resize(_input->nR(),
//			std::vector<std::list<Column const *> >(_input->nB()));
//	_rOrbInColumn.resize(_input->nR(),
//			std::vector<std::list<Column const *> >(_input->nB()));

}
LpMaster::~LpMaster() {
	freeLp();
}
void LpMaster::freeLp() {
	if (_env != NULL) {
		CPXcloseCPLEX(&_env);
		_env = NULL;
	}
	if (_lp != NULL) {
		CPXfreeprob(_env, &_lp);
		_lp = NULL;
	}
}

// on initialise avec les singletons
void LpMaster::initLp() {
	if (_lp != NULL) {
		CPXfreeprob(_env, &_lp);
		_lp = NULL;
	}
	int err(0);
	_lp = CPXcreateprob(_env, &err, "LpMaster");
	// autant de ligne que d'�l�ments
	std::vector<double> const ones(_input->nV(), 1);
	std::vector<int> rmatbeg(_input->nV() + 1, 0);
	//	std::vector<int> rmatind;
	//	std::vector<double> rmatval;
	//	rmatbeg.reserve(_input->nV() + 1);
	//	rmatind.reserve(_input->nV());
	//	rmatval.reserve(_input->nV());
	//	for (size_t i(0); i < _input->nV(); ++i) {
	//		rmatbeg.push_back((int) rmatval.size());
	//		rmatval.push_back(1);
	//		rmatind.push_back((int) i);
	//	}
	//	rmatbeg.push_back((int) rmatval.size());
	//	_primal.resize(_input->nV());
	std::vector<char> const equal(_input->nV(), 'E');
	_dual.resize(_input->nV());

	CPXaddrows(_env, _lp, 0, (int) _input->nV(), 0, ones.data(), equal.data(),
			rmatbeg.data(),
			//			rmatind.data(), rmatval.data(),
			NULL, NULL, NULL, NULL);

	CPXchgobjsen(_env, _lp, -1);
	_index.clear();
	_index.reserve(_primal.size());
	//	size_t i(0);
	//	for (auto const & column : _columns) {
	//		column.id() = i;
	//		_index.push_back((int) i);
	//		for (auto const & r : column.r()) {
	//			for (size_t b(0); b < _input->nB(); ++b)
	//				_rOrbInColumn[r][b].push_back(&column);
	//		}
	//		for (size_t r(0); r < _input->nR(); ++r) {
	//			for (auto const & b : column.b()) {
	//				_rOrbInColumn[r][b].push_back(&column);
	//			}
	//		}
	//		++i;
	//	}
}
void LpMaster::write(std::string const & fileName) const {
	CPXwriteprob(_env, _lp, fileName.c_str(), "LP");
}

void LpMaster::add(Column const & column) {
	//writeColumns("bug.txt");
	auto result(_columns.insert(column));
	if (!result.second) {
//		std::cout << "column already here : " << result.first->id()
//				<< std::endl;
//		result.first->print();
//		double obj;
//		CPXgetobj(_env, _lp, &obj, (int) result.first->id(),
//				(int) result.first->id());
//		std::cout << "column cost is " << obj << std::endl;
//		std::cout << "column violation is "
//				<< result.first->violation(*_decisions) << std::endl;
//		exit(0);
	} else {
		result.first->id() = _primal.size();
		size_t ccnt(1);
		size_t nzcnt(column.size());
		std::vector<double> obj(ccnt, (double) column.cost());
		std::vector<int> cmatbeg(ccnt + 1);
		cmatbeg[0] = 0;
		cmatbeg[1] = (int) nzcnt;
		std::vector<int> rmatind;
		std::vector<double> rmatval(nzcnt, 1);
		std::vector<double> lb(ccnt, 0);
		std::vector<double> ub(ccnt, CPX_INFBOUND);

		cmatbeg.reserve(ccnt + 1);
		rmatind.reserve(nzcnt);

		for (auto const & r : column.r()) {
			//		MY_PRINT(r);
			rmatind.push_back((int) r);
		}
		for (auto const & b : column.b()) {
			//		MY_PRINT(b);
			rmatind.push_back((int) (_input->nR() + b));
		}

		//	MY_PRINT(ccnt);
		//	MY_PRINT(nzcnt);
		//	MY_PRINT(obj.size());
		//	MY_PRINT(obj[0]);
		//	MY_PRINT(cmatbeg.size());
		//	MY_PRINT(cmatbeg[0]);
		//	MY_PRINT(cmatbeg[1]);
		//	MY_PRINT(rmatind.size());
		//	MY_PRINT(rmatind[0]);
		//	MY_PRINT(rmatval.size());
		//	MY_PRINT(rmatval[0]);
		//	MY_PRINT(lb.size());
		//	MY_PRINT(lb[0]);
		//	MY_PRINT(ub.size());
		//	MY_PRINT(ub[0]);

		CPXaddcols(_env, _lp, (int) ccnt, (int) nzcnt, obj.data(),
				cmatbeg.data(), rmatind.data(), rmatval.data(), lb.data(),
				ub.data(), NULL);

		_primal.resize(_primal.size() + ccnt);
		_index.push_back((int) result.first->id());
		std::vector<bool> _rIn(_input->nR(), false);
		for (auto const & r : result.first->r())
			_rIn[r].flip();

		std::vector<bool> _bIn(_input->nB(), false);
		for (auto const & b : result.first->b())
			_bIn[b].flip();

//	for (size_t r(0); r < _input->nR(); ++r) {
//		for (size_t b(0); b < _input->nB(); ++b) {
//			if (_rIn[r] && _bIn[b])
//				_rAndbInColumn[r][b].push_back(&*result.first);
//			else if (!_rIn[r] && _bIn[b])
//				_rOrbInColumn[r][b].push_back(&*result.first);
//			else if (_rIn[r] && !_bIn[b])
//				_rOrbInColumn[r][b].push_back(&*result.first);
//		}
//	}
	}
}
void LpMaster::addSingleton() {
	DoubleVector emptyDual(_input->nV(), 0);
	for (size_t r(0); r < _input->nR(); ++r) {
		Column column(_input);
		column.addElement(1 + r);
		column.cost() = 0;
		column.reducedCost() = column.cost();
		add(column);
	}
	for (size_t b(0); b < _input->nB(); ++b) {
		Column column(_input);
		column.addElement(1 + _input->nR() + b);
		column.cost() = 0;
		column.reducedCost() = column.cost();
		add(column);
	}
}
void LpMaster::addEdge() {
	for (auto const & e : _input->edges()) {
		Column column(_input);
		column.addElement(1 + e._i);
		column.addElement(1 + _input->nR() + e._j);
		column.cost() = _input->w(e._i, e._j);
		column.reducedCost() = column.cost();
		add(column);
	}
}

void LpMaster::readColumns(std::string const & fileName) {
	std::ifstream file(fileName.c_str());
	std::string line;
	size_t i;
	size_t old(_columns.size());
	while (std::getline(file, line)) {
		if (!line.empty()) {
			Column column(_input);
			std::stringstream buffer(line);
			while (buffer >> i) {
				column.addElement(i);
			}
			column.cost() = column.computeCost();
			column.reducedCost() = column.cost();
			add(column);
		}
	}
	file.close();
	std::cout << "Read " << _columns.size() - old << " columns from file "
			<< fileName << std::endl;
}

void LpMaster::getSolution() {
	CPXgetpi(_env, _lp, _dual.data(), 0, (int) (_dual.size() - 1));
	for (size_t i(0); i < _dual.size(); ++i) {
		_dual[i] *= -1;
		//if(std::fabs(_dual[i])<1e-10)
		//	_dual[i] = 0;
	}
	CPXgetx(_env, _lp, _primal.data(), 0, (int) (_primal.size() - 1));
	CPXgetobjval(_env, _lp, &_obj);
}

bool LpMaster::getSolution(FractionnarySolution & solution) {
	solution.clear();
	bool is_integer(true);
	for (auto const & column : _columns) {
		if (std::fabs(_primal[column.id()]) > 1e-6) {
			solution[&column] = _primal[column.id()];
			if (std::fabs(_primal[column.id()]) < 1 - 1e-6) {
				is_integer = false;
//				std::cout << "not integeral " << _primal[column.id()]
//						<< std::endl;
			}
		}
	}
//	MY_PRINT(is_integer);

	return is_integer;
}

void LpMaster::solveMaster() {
	//write();
	CPXlpopt(_env, _lp);
	getSolution();
}

void LpMaster::writeColumns(std::string const & fileName) const {
	std::ofstream file(fileName.c_str());
	for (auto const & column : _columns) {
		for (auto const & r : column.r())
			file << std::setw(6) << r + 1;
		for (auto const & b : column.b())
			file << std::setw(6) << _input->nR() + b + 1;
		file << std::endl;
	}
	file.close();
}

std::set<Column> const & LpMaster::columns() const {
	return _columns;
}

std::vector<double> const & LpMaster::dual() const {
	return _dual;
}

std::vector<double> const & LpMaster::primal() const {
	return _primal;
}

Double LpMaster::obj() const {
	return _obj;
}

void LpMaster::columnsCost(std::vector<double> & obj) {
	obj.resize(_primal.size());
	for (auto const & column : _columns) {
		obj[column.id()] = column.cost();
//		assert(std::fabs(column.cost() - column.computeCost()) < 1e-6);
	}
}

void LpMaster::applyBranchingRule() {
	std::vector<double> obj;
	columnsCost(obj);
//	if (!node.isRoot())
//		applyBranchingRule(node, obj);
	for (auto const & column : _columns) {
		if (column.violation(*_decisions) > 0) {
			obj[column.id()] = -1e6;
		}
	}
	CPXchgobj(_env, _lp, (int) _primal.size(), _index.data(), obj.data());
}
void LpMaster::applyBranchingRule(std::vector<double> & obj) {
	assert(false);
//	std::list<Column const *> const & columns(
//			node.decision().cannot() ?
//					_rAndbInColumn[node.decision().r()][node.decision().b()] :
//					_rOrbInColumn[node.decision().r()][node.decision().b()]);
//	for (auto const & ptr : columns) {
//		obj[ptr->id()] = -1e6;
//	}
//	if (!node.father()->isRoot())
//		applyBranchingRule(*node.father());
}

void LpMaster::branchingWeights(FractionnarySolution const & solution,
		BranchingWeights & weights) {
	// on cherche des arrêtes présentes et semi-présentes dans deux colonnes
	BranchingWeights2 temp;
	std::map<Edge, std::pair<IntSet, IntSet> > toto;
	for (auto const & kvp : solution) {
//		std::cout << std::setw(6) << kvp.first->id();
//		std::cout << std::setw(15) << kvp.second;
//		std::cout << std::endl;
		for (Edge const & e : _input->edges()) {
			bool const iR(kvp.first->constainsR(e._i));
			bool const iB(kvp.first->constainsB(e._j));
			if (iR && iB) {
				toto[e].first.insert(kvp.first->id());
			} else if (iR + iB == 1) {
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
			for (size_t r(0); r < _input->nR(); ++r) {
				for (size_t b(0); b < _input->nB(); ++b) {
					bool const iR(kvp.first->constainsR(r));
					bool const iB(kvp.first->constainsB(b));
					if (iR && iB) {
						toto[Edge(r, b, 1)].first.insert(kvp.first->id());
					} else if (iR + iB == 1) {
						toto[Edge(r, b, 1)].second.insert(kvp.first->id());
					}
				}
			}
		}
		std::cout << "weights.empty()" << std::endl;

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
	}
}

void LpMaster::checkCost(DecisionList const & decisions) const {
//	std::vector<double> obj(_columns.size());
//	CPXgetobj(_env, _lp, obj.data(), 0, obj.size() - 1);
//	for(auto const & column : _columns){
//
//	}

}
void LpMaster::add(ModularityBPartition const & solution) {

	for (size_t const & label : solution.usedLabels()) {
		Column c(_input);
		for (size_t const & n : solution.observations(label)) {
			c.addElement(n + 1);
		}
		c.cost() = c.computeCost();
		add(c);
	}

}
void LpMaster::add(ModularityBPartition const * solution) {
	if (solution != NULL) {
		add(*solution);
	}
}
