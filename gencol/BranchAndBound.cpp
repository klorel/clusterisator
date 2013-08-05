#include "BranchAndBound.hpp"
#include "BipartiteGraph.hpp"
#include "MipGenerator.hpp"
#include "VnsGenerator.hpp"
#include "Timer.hpp"
#include <cplex.h>
#include "LpMaster.hpp"

BranchAndBound::BranchAndBound(BipartiteGraph const &input):_input(input), _master(_input), _vnsGenerator(_input, _master.dual()), _mipGenerator(_input, _master.dual()){
}

BranchAndBound::~BranchAndBound(){
}

void BranchAndBound::columnGeneration(){
	bool stop(false);

	Timer timer;
	Double m(0);
	Double h(0);
	Double e(0);
	Timer total;
	do{
		//write();
		timer.restart();
		_master.solveMaster();
		m+=timer.elapsed();
		timer.restart();
		bool heuristicSucceeded(_vnsGenerator.run());
		h+=timer.elapsed();
		if(heuristicSucceeded){
			_rd = -1;
			size_t p(0);
			for(auto const & column : _vnsGenerator.columns()){
				_master.add(column);
				_rd = std::max(_rd, column.reducedCost() );
				++p;
				if(p>100)
					break;
			}
		}else{
			timer.restart();
			//stop = !generate();
			stop = !_mipGenerator.generate();
			e+=timer.elapsed();
			if(!stop){
				_rd = -1;
				for(auto const & column : _mipGenerator.columns()){
					_master.add(column);
					_rd = std::max(_rd, column.reducedCost() );
				}
			}else
				_rd = _mipGenerator.bestReducedCost();
		}
		std::cout << std::setw(20) << _vnsGenerator.columns().size();
		std::cout << std::setw(20) << _master.obj();
		std::cout << std::setw(20) << _rd;
		std::cout << std::setw(20) << m;
		std::cout << std::setw(20) << h;
		std::cout << std::setw(20) << e;
		std::cout << std::setw(20) << m+h+e;
		std::cout << std::setw(20) << total.elapsed();
		std::cout << std::endl;
	}while(!stop);
}
