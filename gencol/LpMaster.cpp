#include "LpMaster.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include "Node.hpp"

#include <cplex.h>

LpMaster::LpMaster(BipartiteGraph const &input):_input(input), _lp(NULL),  _env(NULL){
	int err;
	_env = CPXopenCPLEX( &err );
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	initLp();
	_rAndbInColumn.resize(_input.nR(),  std::vector<std::list<Column const *> >(input.nB()));
	_rOrbInColumn.resize(_input.nR(),  std::vector<std::list<Column const *> >(input.nB()));
}

LpMaster::~LpMaster(){
	freeLp();
}
void LpMaster::freeLp(){
	if(_env != NULL){
		CPXcloseCPLEX( &_env);
		_env=NULL;
	}
	if(_lp != NULL){
		CPXfreeprob( _env, &_lp );
		_lp=NULL;
	}
}

// on initialise avec les singletons
void LpMaster::initLp(){
	if(_lp != NULL){
		CPXfreeprob( _env, &_lp );
		_lp=NULL;
	}
	int err(0);
	_lp  = CPXcreateprob( _env, &err, "LpMaster" );
	// autant de ligne que d'éléments
	DoubleVector const ones( _input.nV(), 1);
	std::vector<int>	rmatbeg;
	std::vector<int>	rmatind;
	DoubleVector		rmatval;
	rmatbeg.reserve(_input.nV()+1);
	rmatind.reserve(_input.nV());
	rmatval.reserve(_input.nV());
	for(size_t i(0); i<_input.nV(); ++i){
		rmatbeg.push_back(rmatval.size());
		rmatval.push_back(1);
		rmatind.push_back(i);

	}
	rmatbeg.push_back(rmatval.size());
	std::vector<char> const equal( _input.nV(), 'E');
	_dual	.resize(_input.nV());
	_primal	.resize(_input.nV());
	CPXaddrows(_env,
		_lp,
		_input.nV(), 
		_input.nV(),
		_input.nV(),
		ones.data(),
		equal.data(),
		rmatbeg.data(),
		rmatind.data(),
		rmatval.data(),
		NULL,
		NULL
		);
	CPXchgobjsen(_env, _lp, -1);
	_index.clear();
	_index.reserve(_primal.size());	
	size_t i(0);
	// données liées au singleton
	for(auto const & column : _columns){
		column.id() = i;
		_index.push_back(i);
		for(auto const & r : column.first){
			for(size_t b(0); b<_input.nB(); ++b)
				_rOrbInColumn[r][b].push_back(&column);
		}
		for(size_t r(0); r<_input.nR(); ++r){
			for(auto const & b : column.second){
				_rOrbInColumn[r][b].push_back(&column);
			}
		}
		++i;
	}
}
void LpMaster::write(std::string const & fileName)const{
	CPXwriteprob(_env, _lp, fileName.c_str(), "LP");
}

void LpMaster::add(Column const & column){
	//writeColumns("bug.txt");
	auto result(_columns.insert(column));
	if(!result.second){
		std::cout << " column.reducedCost() is " << std::setprecision(20)<<column.reducedCost() << std::endl;
		writeColumns("bug.txt");
		std::cout << "column already here!"<<std::endl;
		column.print();
		_columns.find(column)->print();
		exit(0);
	}
	result.first->id() = _primal.size();
	size_t ccnt(1);
	size_t nzcnt(column.size());
	std::vector<double> obj( ccnt, column.cost());
	std::vector<int> cmatbeg(2);
	cmatbeg[0] = 0;
	cmatbeg[1] = nzcnt;
	std::vector<int> cmatind;
	std::vector<double> cmatval(nzcnt, 1);
	std::vector<double> lb(ccnt, 0);
	std::vector<double> ub(ccnt, CPX_INFBOUND);

	cmatbeg.reserve(ccnt+1);
	cmatind.reserve(nzcnt);

	for(auto const & r : column.first){
		cmatind.push_back(r);
	}
	for(auto const & b : column.second){
		cmatind.push_back(_input.nR()+b);
	}

	CPXaddcols(_env,
		_lp, 
		ccnt, 
		nzcnt, 
		&obj.front(), 
		cmatbeg.data(), 
		&cmatind.front(), 
		&cmatval.front(), 
		lb.data(), 
		ub.data(), 
		NULL);
	_primal.resize(_primal.size()+ccnt);
	_index.push_back(result.first->id());
	std::vector<bool> _rIn(_input.nR(), false);
	for(auto const & r : result.first->first)
		_rIn[r].flip();

	std::vector<bool> _bIn(_input.nB(), false);
	for(auto const & b : result.first->second)
		_bIn[b].flip();

	for(size_t r(0); r<_input.nR(); ++r){
		for(size_t b(0); b<_input.nB(); ++b){
			if( _rIn[r] && _bIn[b] )
				_rAndbInColumn[r][b].push_back( & *result.first);
			else if( !_rIn[r] && _bIn[b] )
				_rOrbInColumn[r][b].push_back( & *result.first);
			else if( _rIn[r] && !_bIn[b] )
				_rOrbInColumn[r][b].push_back( & *result.first);
		}
	}
}
void LpMaster::readColumns(std::string const & fileName){
	std::ifstream file(fileName.c_str());
	std::string line;
	size_t i;
	while(std::getline(file, line) ){
		if(!line.empty()){
			Column column(_input);
			std::stringstream buffer(line);
			while( buffer >> i){
				column.addElement(i);
			}
			column.computeCost();
			add(column);
		}
	}
	file.close();
}

void LpMaster::getSolution(){
	CPXgetpi(_env, _lp, _dual.data(), 0, _dual.size()-1);
	for(size_t i(0); i<_dual.size(); ++i){
		_dual[i]*=-1;
		//if(std::fabs(_dual[i])<1e-10)
		//	_dual[i] = 0;
	}
	CPXgetx (_env, _lp, _primal.data(), 0,_primal.size()-1);
	CPXgetobjval(_env, _lp, &_obj);
}

bool LpMaster::getSolution(FractionnarySolution & solution){
	solution.clear();
	bool is_integer(true);
	for(auto const & column : _columns){
		if( std::fabs(_primal[column.id()])>1e-6){
			solution[&column] = _primal[column.id()];
			if( std::fabs(_primal[column.id()])<1-1e-6){
				is_integer = false;
			}
		}
	}
	return is_integer;
}

void LpMaster::solveMaster(){
	//write();
	CPXlpopt(_env, _lp);
	getSolution();
}


void LpMaster::writeColumns(std::string const & fileName)const{
	std::ofstream file(fileName.c_str());
	for(auto const & column : _columns){
		for(auto const & r : column.first)
			file << std::setw(6)<<r+1;		
		for(auto const & b : column.second)
			file << std::setw(6)<<_input.nR()+b+1;
		file << std::endl;
	}
	file.close();
}


std::set<Column > const & LpMaster::columns()const{
	return _columns;
}

DoubleVector const & LpMaster::dual()const{
	return _dual;
}

DoubleVector const & LpMaster::primal()const{
	return _primal;
}

Double LpMaster::obj()const{
	return _obj;
}


void LpMaster::columnsCost(DoubleVector & obj){
	obj.resize(_primal.size());
	for(auto const & column : _columns){
		obj[column.id()] = column.cost();
	}
}


void LpMaster::applyBranchingRule(Node const & node){
	DoubleVector obj;
	columnsCost(obj);
	applyBranchingRule(node, obj);
	CPXchgobj(_env, _lp, _primal.size(), _index.data(), obj.data());
}
void LpMaster::applyBranchingRule(Node const & node, DoubleVector & obj){
	// on pénalise les colonnes violant les règles
	std::list<Column const * > const & columns(node.cannot() ? _rAndbInColumn[node.r()][node.b()] : _rOrbInColumn[node.r()][node.b()]);
	for(auto const & ptr : columns){
		obj[ptr->id()] = -1e6;
	}
	if(node.father() != NULL)
		applyBranchingRule(*node.father());
}


void LpMaster::branchingWeights(FractionnarySolution const & solution, BranchingWeights & weights){
	BranchingWeights2 temp;
	for(auto const & kvp : solution){
		Double const weight((0.5-std::fabs(kvp.second-0.5))/0.5);
		std::cout << std::setw(6)<<kvp.first->id();
		std::cout << std::setw(25)<<kvp.second;
		std::cout << std::setw(25)<<weight;
		std::cout << std::endl;
		if(weight > 1e-6){
			for(auto const & r : kvp.first->first){
				for(auto const & b : kvp.first->second){
					temp[std::make_pair(r,b)]+=weight;
				}
			}
			//for(auto const & r : kvp.first->first){
			//	for(size_t b(0); b<_input.nB(); ++b){
			//		temp[std::make_pair(r,b)]+=weight;
			//	}
			//}


			//for(size_t r(0); r<_input.nR(); ++r){
			//	for(auto const & b : kvp.first->second){
			//		temp[std::make_pair(r,b)]+=weight;
			//	}
			//}
		}
	}
	weights.clear();
	for(auto const & it : temp){
		weights.insert(std::make_pair(it.second, it.first));
	}
	//for(auto const & w : weights){
	//	std::cout << std::setw(6) << w.second.first;
	//	std::cout << std::setw(6) << w.second.second;
	//	std::cout << std::setw(20) << w.first;
	//	std::cout << std::endl;
	//}
}