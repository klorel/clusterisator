#include "MipGenerator.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>

MipGenerator::MipGenerator(BipartiteGraph const &input, DoubleVector const & dual):_env(NULL),  _oracle(NULL), _input(input), _dual(dual){
	int err;
	_env = CPXopenCPLEX( &err );
	CPXsetintparam(_env, CPX_PARAM_SCRIND, CPX_OFF);
	CPXsetintparam(_env, CPX_PARAM_THREADS,  1);
	CPXsetintparam(_env, CPX_PARAM_CUTPASS, -1);
	CPXsetintparam(_env, CPX_PARAM_VARSEL, 4);
	initOracle();
}

MipGenerator::~MipGenerator(){
	freeLp();
}
void MipGenerator::freeLp(){
	if(_env != NULL){
		CPXcloseCPLEX( &_env);
		_env=NULL;
	}
	if(_oracle != NULL){
		CPXfreeprob( _env, &_oracle );
		_oracle=NULL;
	}
}

void MipGenerator::initOracle(){
	int err(0);
	if(_oracle != NULL){
		CPXfreeprob( _env, &_oracle );
		_oracle=NULL;
	}
	_oracle  = CPXcreateprob( _env, &err, "Oracle" );
	//CPXsetintparam(_env, CPX_PARAM_THREADS, 0);
	size_t const nRB(_input.nR()*_input.nB());


	// variables
	size_t ccnt( _input.nV()+_input.nR()*_input.nB());
	std::vector<int>	cmatbeg(ccnt+1, 0);
	std::vector<double> lb( ccnt, 0);
	std::vector<double> ub( ccnt, 1);
	std::vector<int> index(_input.nV(), -1);
	for(size_t i(0); i<_input.nV(); ++i)
		index[i]=i;
	_index = index;
	std::vector<char> ctype(_input.nV(), 'B');

	std::vector<std::string> _colNames(ccnt);
	std::vector<char *> _cpxColNames;
	_cpxColNames.reserve(ccnt);
	for(size_t r(0); r<_input.nR(); ++r){
		_colNames[r] = GetStr("yR_",r+1);
		_cpxColNames.push_back(const_cast<char*>( _colNames[r].c_str()) );
	}
	for(size_t b(0); b<_input.nB(); ++b){
		_colNames[_input.nR()+b] = GetStr("yB_", b+1);
		_cpxColNames.push_back(const_cast<char*>( _colNames[_input.nR()+b].c_str()) );
	}
	for(size_t r(0); r<_input.nR(); ++r){
		for(size_t b(0); b<_input.nB(); ++b){
			_colNames[_input.nV()+r*_input.nB()+b] = GetStr( "yRB_", r+1, "_", b+1);
			_cpxColNames.push_back(const_cast<char*>( _colNames[_input.nV()+r*_input.nB()+b].c_str()) );
		}
	}

	CPXaddcols(_env, 
		_oracle,
		ccnt,
		0, 
		NULL,
		cmatbeg.data(),
		NULL, 
		NULL,
		lb.data(),
		ub.data(), 
		//NULL
		_cpxColNames.data()
		);
	// binary declaration
	CPXchgctype(_env, _oracle, index.size(), index.data(), ctype.data());
	std::vector<double> obj;
	index.clear();
	obj		.reserve(nRB);
	index	.reserve(nRB);
	for(size_t r(0); r<_input.nR(); ++r){
		for(size_t b(0); b<_input.nB(); ++b){
			size_t const yRB(_input.nV()+r*_input.nB()+b);
			obj		.push_back (_input.w(r,b));
			index	.push_back (yRB);						
		}
	}
	CPXchgobj(_env, _oracle, obj.size(), index.data(), obj.data());
	// ob of linearize variables

	// constraints
	size_t const rcnt( 3*nRB);
	std::vector<double>	rhs;
	std::vector<char>	rtype;
	std::vector<int>	rmatbeg;
	std::vector<int>	rmatind;
	std::vector<double>	rmatval;
	std::vector<std::string> _rowNames;
	std::vector<char *> _rowColNames;
	_rowNames.reserve(rcnt);
	_rowColNames.reserve(rcnt);
	// for the linearization of the product
	//size_t rnzcnt(0);
	rhs		.reserve( rcnt);
	rtype	.reserve( rcnt);		
	rmatbeg	.reserve( rcnt);
	rmatind	.reserve(7*nRB);
	rmatval	.reserve(7*nRB);

	for(size_t r(0); r<_input.nR(); ++r){
		size_t const yR(r);
		for(size_t b(0); b<_input.nB(); ++b){
			size_t const yB(_input.nR()+b);
			size_t const yRB(_input.nV()+r*_input.nB()+b);
			// yRB - yR <= 0
			rmatbeg.push_back(rmatval.size());
			rmatind.push_back(yR);
			rmatval.push_back(-1);
			rmatind.push_back(yRB);
			rmatval.push_back(1);
			rtype.push_back('L');
			rhs.push_back(0);
			// yRB - yB <= 0
			rmatbeg.push_back(rmatval.size());
			rmatind.push_back(yB);
			rmatval.push_back(-1);
			rmatind.push_back(yRB);
			rmatval.push_back(1);
			rtype.push_back('L');
			rhs.push_back(0);
			// yRB - yR - yB >= -1
			rmatbeg.push_back(rmatval.size());
			rmatind.push_back(yR);
			rmatval.push_back(-1);
			rmatind.push_back(yB);
			rmatval.push_back(-1);
			rmatind.push_back(yRB);
			rmatval.push_back(1);
			rtype.push_back('G');
			rhs.push_back(-1);
		}
	}
	rmatbeg.push_back(rmatval.size());
	CPXaddrows(_env,
		_oracle,
		0,
		rhs.size(),
		rmatval.size(),
		rhs.data(),
		rtype.data(),
		rmatbeg.data(),
		rmatind.data(),
		rmatval.data(),
		NULL,
		NULL
		);
	CPXchgobjsen(_env, _oracle, -1);
}
// on initialise avec les singletons
void MipGenerator::writeOracle(std::string const & fileName)const{
	CPXwriteprob(_env, _oracle, fileName.c_str(), "LP");
}

void MipGenerator::setUpOracle(){
	_columns.clear();
	CPXchgobj(_env, _oracle, _index.size(), _index.data(), _dual.data());
	//writeOracle();
	if(CPXgetnummipstarts(_env, _oracle)>1)
		CPXdelmipstarts(_env, _oracle, 0, CPXgetnummipstarts(_env, _oracle)-1);
}
Double MipGenerator::bestReducedCost()const{
	return _bestReducedCost;
}

bool MipGenerator::generate(){
	setUpOracle();
	CPXmipopt(_env, _oracle);

	CPXgetobjval(_env, _oracle, &_bestReducedCost);
	//_rd-=_cstDual;
	bool const result(_bestReducedCost>1e-6);
	if(result){
		DoubleVector x( _input.nV()+_input.nR()*_input.nB());	
		CPXgetx (_env, _oracle, x.data(), 0, x.size()-1);
		// verification
		//DoubleVector obj(_input.nV()+_input.nR()*_input.nB());
		//CPXgetobj(_env, _oracle, obj.data(), 0, obj.size()-1);
		//Double cost(0);
		//for(size_t r(0); r<_input.nR(); ++r){
		//	size_t const yR(r);
		//	for(size_t b(0); b<_input.nB(); ++b){
		//		size_t const yB(_input.nR()+b);
		//		size_t const yRB(_input.nV()+r*_input.nB()+b);
		//		assert( std::fabs( x[yR]*x[yB]-x[yRB] )<1e-10);
		//		cost+= x[yRB] * obj[yRB];
		//	}
		//}
		//Double reducedCost(cost);
		Column column(_input);
		for(size_t r(0); r<_input.nR(); ++r){
			if (x[r]>0.5){
				column.first.insert(r);
			}
		}
		for(size_t b(0); b<_input.nB(); ++b){
			if (x[_input.nR()+b]>0.5){
				column.second.insert(b);
			}
		}
		column.computeCost();
		_columns.insert(column);
	}
	return result;
}


std::set<Column > const & MipGenerator::columns()const{
	return _columns;
}