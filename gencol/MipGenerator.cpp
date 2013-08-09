#include "MipGenerator.hpp"
#include "BipartiteGraph.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>

MipGenerator::MipGenerator(BipartiteGraph const &input, DoubleVector const & dual):_env(NULL),  _oracle(NULL), _input(input), _dual(dual){
	int err;
	_env = CPXopenCPLEX( &err );
	CPXsetintparam(_env, CPX_PARAM_SCRIND,		CPX_OFF);
	//CPXsetintparam(_env, CPX_PARAM_SCRIND,		CPX_ON);
	CPXsetintparam(_env, CPX_PARAM_THREADS,		1);
	CPXsetintparam(_env, CPX_PARAM_CUTPASS,		-1);
	CPXsetintparam(_env, CPX_PARAM_VARSEL,		4);
	CPXsetintparam(_env, CPX_PARAM_MIPDISPLAY,	2);
	//CPXsetintparam(_env, CPX_PARAM_MIPEMPHASIS,	CPX_MIPEMPHASIS_BESTBOUND);
	
	//initOracle();
	initOracle2();
	//writeOracle();
	//CPXmipopt(_env, _oracle);
	//double toto;
	//CPXgetobjval(_env, _oracle, &toto);
	//std::cout << "toto : "<<toto<<std::endl;

	//CPXsetdblparam(_env, CPX_PARAM_CUTLO, 1e-6);
	//CPXsetdblparam(_env, CPX_PARAM_CUTUP, 1e-6);
	//CPXsetintparam(_env, CPX_PARAM_POPULATELIM , 1);
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
	std::vector<int> cindex(_input.nV(), -1);
	for(size_t i(0); i<_input.nV(); ++i)
		cindex[i]=i;
	_index = cindex;
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
	CPXchgctype(_env, _oracle, cindex.size(), cindex.data(), ctype.data());
	std::vector<double> obj;
	cindex.clear();
	obj		.reserve(nRB);
	cindex	.reserve(nRB);
	for(size_t r(0); r<_input.nR(); ++r){
		for(size_t b(0); b<_input.nB(); ++b){
			size_t const yRB(_input.nV()+r*_input.nB()+b);
			obj		.push_back (_input.w(r,b));
			cindex	.push_back (yRB);						
		}
	}
	CPXchgobj(_env, _oracle, obj.size(), cindex.data(), obj.data());
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
	//CPXaddlazyconstraints(_env,
	//	_oracle,
	//	rhs.size(),
	//	rmatval.size(),
	//	rhs.data(),
	//	rtype.data(),
	//	rmatbeg.data(),
	//	rmatind.data(),
	//	rmatval.data(),
	//	NULL
	//	);
	CPXchgobjsen(_env, _oracle, -1);
}



void MipGenerator::initOracle2(){
	size_t const nRB(_input.nR()*_input.nB());
	int err(0);
	if(_oracle != NULL){
		CPXfreeprob( _env, &_oracle );
		_oracle=NULL;
	}
	_oracle  = CPXcreateprob( _env, &err, "Oracle" );
	std::vector<double> lb;
	std::vector<double> ub;
	std::vector<int> cindex;
	std::vector<char> ctype;
	std::vector<double> obj;
	std::vector<std::string> cname;
	// 0..R-1 : Yr
	// R..R+B-1 : Yb
	size_t id(0);
	for(; id<_input.nV(); ++id){
		// Y binary
		cindex.push_back( id );
		ctype.push_back(CPX_BINARY);
		lb.push_back(+0);
		ub.push_back(+1);
		obj.push_back(0);
		cname.push_back( GetStr("Y_", id));
	}
	_index = cindex;
	// 
	RectMatrix s(_input.nR(), _input.nB());	
	for(auto const & e : _input.edges()){
		// S reel
		s.get(e._i, e._j)=id;
		cindex.push_back( id );
		ctype.push_back(CPX_CONTINUOUS);
		lb.push_back(-CPX_INFBOUND);
		ub.push_back(+CPX_INFBOUND);
		obj.push_back (e._v*_input.inv_m());
		cname.push_back( GetStr("S_", e._i, "_", e._j));
		++id;
	}
	size_t const tR((size_t)std::ceil((std::log(_input.sum_kR()+1)/log(2)-1)));
	size_t const tB((size_t)std::ceil((std::log(_input.sum_kB()+1)/log(2)-1)));
	IntVector a(tR+1);
	for(size_t h(0); h<tR+1; ++h, ++id){
		// ah binary
		a[h] = id;
		cindex.push_back( id );
		ctype.push_back(CPX_BINARY);
		lb.push_back(+0);
		ub.push_back(+1);
		obj.push_back(0);
		cname.push_back( GetStr("a_",h));		
	}
	IntVector b(tR+1);
	for(size_t l(0); l<tB+1; ++l, ++id){
		// bl binary
		b[l] = id;
		cindex.push_back( id );
		ctype.push_back(CPX_BINARY);
		lb.push_back(+0);
		ub.push_back(+1);
		obj.push_back(0);
		cname.push_back( GetStr("b_",l));
	}
	RectMatrix ab(tR+1, tB+1);
	for(size_t h(0); h<tR+1; ++h){
		for(size_t l(0); l<tB+1; ++l, ++id){
			// ahl >= 0
			ab.get(h,l) = id;
			cindex.push_back( id );
			ctype.push_back(CPX_CONTINUOUS);
			lb.push_back(+0);
			ub.push_back(+CPX_INFBOUND);
			obj.push_back(-std::pow(2, l+h)*_input.inv_m()*_input.inv_m());
			cname.push_back( GetStr("a_",h, "_", l));
		}
	}
	size_t const R1(id);
	cindex.push_back(id);
	ctype.push_back(CPX_CONTINUOUS);
	lb.push_back(-CPX_INFBOUND);
	ub.push_back(+CPX_INFBOUND);
	obj.push_back(0);
	cname.push_back( "R1" );
	++id;
	size_t const B1(id);
	cindex.push_back(id);
	ctype.push_back(CPX_CONTINUOUS);
	lb.push_back(-CPX_INFBOUND);
	ub.push_back(+CPX_INFBOUND);
	obj.push_back(0);
	cname.push_back( "B1" );
	++id;
	size_t ccnt(id);
	std::vector<int>	cmatbeg(ccnt+1, 0);
	//CPXsetintparam(_env, CPX_PARAM_THREADS, 0);
	// variables
	std::vector<char*> cnameCPX(cname.size());
	for(size_t i(0); i<cname.size(); ++i){
		cnameCPX[i] = const_cast<char*>( cname[i].c_str());
	}
	CPXaddcols(_env, 
		_oracle,
		ccnt,
		0, 
		obj.data(),
		cmatbeg.data(),
		NULL, 
		NULL,
		lb.data(),
		ub.data(), 
		//NULL
		cnameCPX.data()
		);
	// binary declaration
	CPXchgctype(_env, _oracle, cindex.size(), cindex.data(), ctype.data());
	
	// constraints
	std::vector<double>	rhs;
	std::vector<char>	rtype;
	std::vector<int>	rmatbeg;
	std::vector<int>	rmatind;
	std::vector<double>	rmatval;
	std::vector<std::string> rname;
	// R1 = sum kr Yr
	rmatbeg.push_back(rmatval.size());
	rname.push_back("R1_k");
	for(size_t r(0); r<_input.nR(); ++r){
		rmatind.push_back(r);
		rmatval.push_back(_input.kR(r));		
	}
	rmatind.push_back(R1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// R1 = sum 2^h ah
	rmatbeg.push_back(rmatval.size());
	rname.push_back("R1_POW");
	for(size_t h(0); h<tR+1; ++h){
		rmatind.push_back(a[h]);
		rmatval.push_back(std::pow(2, h));
	}
	rmatind.push_back(R1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// B1 = sum kb Yb
	rmatbeg.push_back(rmatval.size());
	rname.push_back("B1_k");
	for(size_t b(0); b<_input.nB(); ++b){
		rmatind.push_back(_input.nR()+b);
		rmatval.push_back(_input.kB(b));
	}
	rmatind.push_back(B1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// B1 = sum 2^l al
	rmatbeg.push_back(rmatval.size());
	rname.push_back("B1_POW");
	for(size_t l(0); l<tB+1; ++l){
		rmatind.push_back(b[l]);
		rmatval.push_back(std::pow(2, l));
	}
	rmatind.push_back(B1);
	rmatval.push_back(-1);
	rtype.push_back('E');
	rhs.push_back(0);
	// Srb <= Yr (r,b) in E	
	for(auto const & e : _input.edges()){
		rmatbeg.push_back(rmatval.size());
		rname.push_back(GetStr("SY_", e._i));
		rmatind.push_back(e._i);
		rmatval.push_back(-1);
		rmatind.push_back((int)s.get(e._i, e._j));
		rmatval.push_back(+1);
		rtype.push_back('L');
		rhs.push_back(0);
	}
	// Srb <= Yb (r,b) in E
	for(auto const & e : _input.edges()){
		rmatbeg.push_back(rmatval.size());
		rname.push_back(GetStr("SY_", e._j));
		rmatind.push_back(_input.nR()+e._j);
		rmatval.push_back(-1);
		rmatind.push_back((int)s.get(e._i, e._j));
		rmatval.push_back(+1);
		rtype.push_back('L');
		rhs.push_back(0);
	}
	// abhl >= ah+bl-1
	for(size_t h(0); h<tR+1; ++h){
		for(size_t l(0); l<tB+1; ++l){
			rmatbeg.push_back(rmatval.size());
			rname.push_back( GetStr("FORTET_", h, "_", l));
			rmatind.push_back((int)ab.get(h,l));
			rmatval.push_back(+1);
			rmatind.push_back(a[h]);
			rmatval.push_back(-1);
			rmatind.push_back(b[l]);
			rmatval.push_back(-1);
			rtype.push_back('G');
			rhs.push_back(-1);
		}
	}
	std::vector<char*> rnameCPX(rname.size());
	for(size_t i(0); i<rname.size(); ++i){
		rnameCPX[i] = const_cast<char*>( rname[i].c_str());
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
		rnameCPX.data()
		);
	//CPXaddlazyconstraints(_env,
	//	_oracle,
	//	rhs.size(),
	//	rmatval.size(),
	//	rhs.data(),
	//	rtype.data(),
	//	rmatbeg.data(),
	//	rmatind.data(),
	//	rmatval.data(),
	//	rnameCPX.data()
	//	);
	CPXchgobjsen(_env, _oracle, -1);
	//writeOracle();
	//CPXmipopt(_env, _oracle);
	//double toto;
	//CPXgetobjval(_env, _oracle, &toto);
	//std::cout << "toto : "<<toto<<std::endl;
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
	//CPXmipopt(_env, _oracle);
	CPXpopulate (_env, _oracle);
	bool result(false);
	_bestReducedCost = 1e-6;
	if(
		   CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL
		|| CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL_TOL
		|| CPXgetstat(_env, _oracle) == CPXMIP_POPULATESOL_LIM
		|| CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL_POPULATED
		|| CPXgetstat(_env, _oracle) == CPXMIP_OPTIMAL_POPULATED_TOL
	){
		CPXgetobjval(_env, _oracle, &_bestReducedCost);
		//_rd-=_cstDual;
		std::cout << "_bestReducedCost : "<<_bestReducedCost<<std::endl;
		result = (_bestReducedCost>1e-6);
		if(result){
			DoubleVector x( CPXgetnumcols(_env, _oracle));	
			size_t const n(CPXgetsolnpoolnumsolns(_env, _oracle));
			std::cout << std::setw(4)<<n<<std::endl;
			Double obj;
			for(size_t i(0);i<n; ++i){
				CPXgetsolnpoolobjval(_env, _oracle, i, &obj);
				std::cout << std::setw(4)<<i;
				std::cout << std::setw(25)<<std::setprecision(20)<<obj<<std::endl;
				if(obj>1e-6){
					CPXgetsolnpoolx(_env, _oracle, i,  x.data(), 0, x.size()-1);
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
					column.reducedCost() = obj;
					column.check(_dual);
					_columns.insert(column);
				}
			}

			//CPXgetx (_env, _oracle, x.data(), 0, x.size()-1);
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
			//Column column(_input);
			//for(size_t r(0); r<_input.nR(); ++r){
			//	if (x[r]>0.5){
			//		column.first.insert(r);
			//	}
			//}
			//for(size_t b(0); b<_input.nB(); ++b){
			//	if (x[_input.nR()+b]>0.5){
			//		column.second.insert(b);
			//	}
			//}
			//column.computeCost();
			//column.reducedCost() = _bestReducedCost;
			//_columns.insert(column);
		}
	}else
		std::cout << "CPXgetstat : "<< CPXgetstat(_env, _oracle) << std::endl;
	return result;
}


std::set<Column > const & MipGenerator::columns()const{
	return _columns;
}