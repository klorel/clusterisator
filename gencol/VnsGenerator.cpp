#include "VnsGenerator.hpp"
#include "Node.hpp"

VnsGenerator::VnsGenerator(BipartiteGraph const & input, DoubleVector const & dual):_input(input),_dual(dual), _allNodes(input.nV(), true){
	_graph.resize(_input.nV());	
	for( auto const & e : _input.edges()){
		_graph[e._i].insert(_input.nR()+e._j);
		_graph[_input.nR()+e._j].insert(e._i);
	}

}
VnsGenerator::~VnsGenerator(){
}

void VnsGenerator::shake(VnsGeneratorSolution & solution, size_t k){
	_allNodes.fill();
	for(size_t p(0); p<k && !_allNodes.empty(); ++p){
		size_t const id(_allNodes.pop_random());
		solution.swap(id);
	}
	compute(solution);
}

void VnsGenerator::compute(VnsGeneratorSolution & solution){
	solution._cost = solution.computeCost();
	solution._reducedCost = solution.computeReducedCost();
	//for(auto const & v : solution._v){
	//	solution._reducedCost += _dual[v];
	//}
	//for(auto const & r : solution._r){
	//	for(auto const & b : solution._b){
	//		solution._cost+=_input.w(r, b);
	//	}
	//}
	//solution._reducedCost += solution._cost;
}

bool VnsGenerator::localSearch(VnsGeneratorSolution & solution){
	bool success(false);
	bool stop(false);
	IntVector temp(_input.nV());
	Double deltaCost;
	Double deltaDual;
	Double delta;
	do{
		bool rSucess(false);
		// optimization of blue
		for(size_t r(0); r<_input.nR(); ++r){
			deltaDual  = _dual[r];
			deltaCost  = 0;
			for(auto const & b : solution._b)
				deltaCost +=_input.w(r,b);
			delta = deltaCost+deltaDual;
			if(delta>1e-6 && !solution._r.contains(r)){
				solution.swap(r);
				solution._cost += deltaCost;
				solution._reducedCost += delta;
				rSucess = true;
			} else if(delta<-1e-6 && solution._r.contains(r)){
				solution.swap(r);
				solution._cost -= deltaCost;
				solution._reducedCost -= delta;
				rSucess = true;
			}
		}
		bool bSucess(false);
		for(size_t b(0); b<_input.nB(); ++b){
			deltaDual  =  _dual[_input.nR()+b];
			deltaCost  = 0;
			for(auto const & r : solution._r)
				deltaCost+=_input.w(r,b);
			delta = deltaCost+deltaDual;
			if(delta>1e-6 && !solution._b.contains(b)){
				solution.swap(_input.nR()+b);
				solution._cost += deltaCost;
				solution._reducedCost += delta;
				bSucess = true;
			} else if(delta<-1e-6 && solution._b.contains(b)){
				solution.swap(_input.nR()+b);
				solution._cost -= deltaCost;
				solution._reducedCost -= delta;
				bSucess = true;
			}
		}
		stop = !rSucess && ! bSucess;		
		solution.check();
	}while(!stop);
	//

	//

	return success;
}

void VnsGeneratorSolution::swap(size_t id){
	bool const isR( id < _input.nR());
	if( _v.contains(id)){
		_v.erase(id);
		if(isR)
			_r.erase(id);
		else			
			_b.erase(id-_input.nR());
	}else{
		_v.insert(id);
		if(isR)
			_r.insert(id);
		else			
			_b.insert(id-_input.nR());
	}
}


VnsGeneratorSolution::VnsGeneratorSolution(BipartiteGraph const & input, DoubleVector const & dual):_input(input), _dual(dual), _v(_input.nV()), _r(_input.nR()), _b(_input.nB()){

}

VnsGeneratorSolution::~VnsGeneratorSolution(){
}


Double VnsGeneratorSolution::computeCost()const{
	Double result(0);
	for(auto const & r : _r){
		for(auto const & b : _b){
			result+=_input.w(r,b);
		}
	}
	return result;
}
Double VnsGeneratorSolution::computeReducedCost()const{
	Double result(computeCost());
	for(auto const & r : _r){
		result+=_dual[r];
	}
	for(auto const & b : _b){
		result+=_dual[_input.nR()+b];
	}
	return result;
}
Double VnsGeneratorSolution::delta(size_t id){
	Double links(0);
	if(id<_input.nR()){
		for(auto const & b : _b){
			links+= _input.w(id, b);
		}
	}else{
		for(auto const & r : _r){
			links+= _input.w(r, id-_input.nR());
		}
	}
	if(_v.contains(id))
		return -(links+_dual[id]);
	else
		return +(links+_dual[id]);
}

bool VnsGenerator::run(){
	_columns.clear();
	size_t ite(0);
	size_t iteMax(2);
	size_t k(0);
	Double kMax(_input.nV());
	VnsGeneratorSolution current(_input, _dual);
	VnsGeneratorSolution best(_input, _dual);
	compute(current);
	best = current;
	Column column(_input);
	do{
		++k;
		if(k>=kMax){
			k=1;
			++ite;
		}
		shake(current, k);
		localSearch(current);
		if(current._reducedCost > 1e-6){
			current.build(column);
			_columns.insert(column);
		}
		if( VnsGeneratorSolution::IsBetter(current, best)){
			best=current;
			k=0;
		}else{
			current = best;
		}
	}while(ite<iteMax && _columns.size()<50);
	//if(ite==iteMax)
	//	std::cout << "iteMax"<<std::endl;
	return !_columns.empty();
}
void VnsGeneratorSolution::build(Column & column){
	column.first.clear();
	column.second.clear();
	for(auto const & r : _r){
		column.first.insert(r);
	}
	for(auto const & b : _b){
		column.second.insert(b);
	}
	column.cost() = _cost;
	column.reducedCost() = _reducedCost;
}
VnsGeneratorSolution & VnsGeneratorSolution::operator=(VnsGeneratorSolution const & rhs){
	if(&rhs != this){
		_v = rhs._v;
		_r = rhs._r;
		_b = rhs._b;
		_cost = rhs._cost;
		_reducedCost = rhs._reducedCost;
	}
	return *this;
}


std::set<Column > const & VnsGenerator::columns()const{
	return _columns;
}


size_t VnsGeneratorSolution::violation(Node const & node)const{
	size_t result(0);
	size_t const sum( _r.contains(node.r()) + _b.contains(node.b()));
	if(node.cannot()){
		if(sum > 1)
			result+=1;
	}else{
		if( sum== 1)
			result+=1;
	}
	if(node.father() != NULL)
		result+= violation(*node.father());
	return result;
}
bool VnsGeneratorSolution::IsBetter(VnsGeneratorSolution const & p, VnsGeneratorSolution const & q, Node const & node){
	size_t const pViolation(p.violation(node));
	size_t const qViolation(q.violation(node));
	if( pViolation == qViolation){
		return p._reducedCost > q._reducedCost + 1e-6;
	}else{
		return pViolation < qViolation;
	}
}
bool VnsGeneratorSolution::IsBetter(VnsGeneratorSolution const & p, VnsGeneratorSolution const & q){
	return p._reducedCost > q._reducedCost + 1e-6;
}

void VnsGenerator::sortedColumns(std::multimap<Double, Column const *, std::greater<Double>> & result)const{
	result.clear();
	for(auto const & column : _columns){
		result.insert(std::make_pair(column.reducedCost(), &column));
	}
}

void VnsGeneratorSolution::check()const{
	assert(std::fabs(_cost-computeCost())<1e-10);
	assert(std::fabs(_reducedCost-computeReducedCost())<1e-10);
}