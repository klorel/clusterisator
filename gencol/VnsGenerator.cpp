#include "VnsGenerator.hpp"


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
	solution._cost = 0;;
	solution._reducedCost = 0;
	for(auto const & v : solution._v){
		solution._reducedCost += _dual[v];
	}
	for(auto const & r : solution._r){
		for(auto const & b : solution._b){
			solution._cost+=_input.w(r, b);
		}
	}
	solution._reducedCost += solution._cost;
}

bool VnsGenerator::localSearch(VnsGeneratorSolution & solution){
	bool success(false);
	bool stop(false);
	IntVector temp(_input.nV());
	do{
		temp.clear();
		std::multimap<Double, size_t, std::greater<Double> > sorter;
		for(size_t id(0); id<_input.nV(); ++id){
			sorter.insert(std::make_pair(solution.delta(id), id));
		}
		if(sorter.begin()->first>1e-6)
			success = true;
		else
			stop = true;

		for( auto const & it : sorter){
			if( it.first<sorter.begin()->first)
				break;
			else
				temp.push_back(it.second);
		}
		solution.swap( RandomElement(temp));
		compute(solution);
	}while(!stop);
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
	size_t iteMax(5);
	size_t k(0);
	Double kMax(_input.nV()*0.82+1);
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
		if(current._reducedCost>best._reducedCost){
			best=current;
			k=0;
		}else{
			current = best;
		}
	}while(ite<iteMax && (_columns.empty() || ite==0));
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