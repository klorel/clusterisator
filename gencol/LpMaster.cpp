#include "LpMaster.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>

LpMaster::LpMaster(BipartiteGraph const &input):_input(input), _lp(NULL),  _env(NULL){
	int err;
	_env = CPXopenCPLEX( &err );
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	initLp();
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
}
void LpMaster::write(std::string const & fileName)const{
	CPXwriteprob(_env, _lp, fileName.c_str(), "LP");
}

void LpMaster::add(Column const & column){
		//writeColumns("bug.txt");
	if(!_columns.insert(column).second){
		writeColumns("bug.txt");
		std::cout << "column already here!"<<std::endl;
		exit(0);
	}
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
	for(size_t i(0); i<_dual.size(); ++i)
		_dual[i]*=-1;
	CPXgetx (_env, _lp, _primal.data(), 0,_primal.size()-1);
	CPXgetobjval(_env, _lp, &_obj);
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