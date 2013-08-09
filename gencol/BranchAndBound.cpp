#include "BranchAndBound.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>
#include "LpMaster.hpp"
#include "Node.hpp"

BranchAndBound::BranchAndBound(BipartiteGraph const &input):_input(input), _master(_input), _vnsGenerator(_input, _master.dual()), _mipGenerator(_input, _master.dual()){
	_root = NULL;
}

BranchAndBound::~BranchAndBound(){
	delete _root;
}

void BranchAndBound::columnGeneration(Node * node){
	bool stop(false);

	//_master.readColumns("bug.txt");
	Timer timer;
	Double m(0);
	Double h(0);
	Double e(0);
	Timer total;
	std::multimap<Double, Column const *, std::greater<Double> > sorter;
	std::string step;
	size_t nb;
	do{
		//write();
		timer.restart();
		_master.solveMaster();
		m+=timer.elapsed();
		timer.restart();
		//bool heuristicSucceeded(false);
		bool heuristicSucceeded(_vnsGenerator.run());
		h+=timer.elapsed();

		if(heuristicSucceeded){
			step="HEURISTIC";
			_rd = -1;
			size_t p(0);
			_vnsGenerator.sortedColumns(sorter);
			nb = 0;
			for(auto const & column : sorter){
				_master.add(*column.second);
				_rd = std::max(_rd, column.first );
				++p;
				++nb;
				if(p>=1000)
					break;
			}

			//for(auto const & column : _vnsGenerator.columns()){
			//	_master.add(column);
			//	_rd = std::max(_rd, column.reducedCost() );
			//	++p;
			//	if(p>100)
			//		break;
			//}
		}else{
			step="EXACT";
			nb=0;
			timer.restart();
			//stop = !generate();
			stop = !_mipGenerator.generate();
			e+=timer.elapsed();
			if(!stop){
				_rd = -1;
				for(auto const & column : _mipGenerator.columns()){
					++nb;
					_master.add(column);
					_rd = std::max(_rd, column.reducedCost() );
				}
			}else
				_rd = _mipGenerator.bestReducedCost();
		}
		std::cout << std::setw(20) << step;
		std::cout << std::setw(4) << nb;
		std::cout << std::setw(20) << _master.obj();
		std::cout << std::setw(20) << std::setprecision(10)<< _rd;
		//std::cout << std::setw(20) << m;
		//std::cout << std::setw(20) << h;
		//std::cout << std::setw(20) << e;
		//std::cout << std::setw(20) << m+h+e;
		std::cout << std::setw(20) << total.elapsed();
		std::cout << std::endl;
	}while(!stop);

	FractionnarySolution solution;
	bool is_integer = _master.getSolution(solution);
	std::cout << "is_integer  : "<<is_integer <<std::endl;
	BranchingWeights weights;
	_master.branchingWeights(solution, weights);
	std::cout << weights.size() << " / "<<_input.nR()*_input.nB()<<std::endl;
}



void BranchAndBound:: branch(Node * node, size_t r, size_t b){
	// on crée un nouveau nouveau noeud,
	// on ajoute la contrainte
	// on ajoute le noeud à la pile avec la ub à recalculer ?
}


void BranchAndBound::run(){
	if(_root != NULL)
		delete _root;
	_root = new Node(*this);

	columnGeneration(_root);
}