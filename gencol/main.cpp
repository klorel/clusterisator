/*
* main.cpp
*
*  Created on: 15 déc. 2012
*      Author: manuel
*/

#include "common.h"
#include "Partition.hpp"
#include "Number.hpp"
#include <random>
#include <iostream>
#include <fstream>
#include "RegisteredModularityBInstance.hpp"
#include "BipartiteGraph.hpp"
#include "ModularityBPartition.hpp"


bool optimize(ModularityBPartition & p, Double & score, IntVector & solution){
	bool success(false);
	score = p.computeScore();
	//std::cout << std::setw(4) << p.nbLabels();
	//std::cout << std::setw(20) <<score;
	//std::cout << std::endl;
	bool goOn(true);
	do{		
		goOn = false;
		for(auto const & e : p.input().edges()){
			if( solution[e._i]!=solution[e._j]){
				p.fusion(solution[e._i], solution[e._j]);
			}else if(!p.allLabelsUsed()){
				{
					size_t const newLabel(p.getUnUsedLabel());
					size_t const moved(e._i);
					for(auto const & n : p.input()(moved)){
						if(n.first != moved && solution[moved]==solution[n.first])
							p.shift(n.first,newLabel);
					}
					p.shift(moved,newLabel);
				}
				if(!p.allLabelsUsed()){
					size_t const newLabel (p.getUnUsedLabel());
					size_t const moved( e._j);
					for(auto const & n : p.input()(moved)){
						if(n.first != moved && solution[moved]==solution[n.first])
							p.shift(n.first,newLabel);
					}
					p.shift(moved,newLabel);
				}
			}
			Double const candidate(p.computeScore());
			if( candidate > score + 1e-6){
				score = candidate;
				solution = p.labels();
				goOn=true;
				success = true;
				//std::cout << std::setw(4) << p.nbLabels();
				//std::cout << std::setw(20) <<score;
				//std::cout << std::endl;
			}else
				p.setLabels(solution);	
		}	
	}while(goOn);
	return success;
}

int main(int argc, char** argv){
	RegisteredModularityBInstance::InstancesPath = "C:\\Users\\manuel\\Documents\\Github\\clusterisator\\bipartite_instances\\";
	RegisteredModularityBInstance instance(women);
	instance.out();
	ModularityBPartition p(instance, instance.nbNodes()); 
	p.random();
	//RegisteredModularityBInstance::InstancesPath +"sol.txt">>p;
	std::cout << std::setw(4) << p.nbLabels();
	std::cout << std::setw(20) <<p.computeScore();
	std::cout << std::endl;
	//p.random();
	size_t ite(0);
	size_t k(0);
	size_t const kMax(instance.nbNodes());
	Double score;
	IntVector current(p.labels());
	optimize(p, score, current);

	Double best(score);
	IntVector solution(current);
	//std::ofstream file((RegisteredModularityBInstance::InstancesPath+"sol.txt").c_str());
	//for(auto const & e : p.input().edges()){
	//	if(solution[e._i]==solution[e._j]){
	//		size_t const i(std::min(e._i, e._j));
	//		size_t const j(std::max(e._i, e._j));
	//		file << "let x["<<i+1;
	//		file << ", "<<j+1-instance.nR();
	//		file << "] := "<<(solution[e._i]==solution[e._j]) << ";"<<std::endl;
	//	}
	//}
	//file.close();
	std::cout << std::setw(4) << k;
	std::cout << std::setw(4) << p.nbLabels();
	std::cout << std::setw(20)<< score;
	std::cout << std::endl;
	do{
		//std::cout <<"---------------"<<std::endl;
		++ite;
		++k;
		k=(k>kMax?1:k);
		if(!p.allLabelsUsed()){
			//size_t const newLabel(p.getUnUsedLabel());
			//for(size_t n(0); n<k && !p.allLabelsUsed(); ++n){
			//	size_t const node(Number::Generator()%instance.nbNodes());
			//	if(p.sizeOfLabel(p.label(node))>1)
			//		p.shift(node,newLabel);
			//}
			for(size_t n(0); n<k && !p.allLabelsUsed(); ++n){
				size_t const node(Number::Generator()%instance.nbNodes());
				if(p.sizeOfLabel(p.label(node))>1)
					p.shift(node,p.getUnUsedLabel());
			}
			//std::cout << std::setw(8) << k;
			//std::cout << std::setw(4) << p.nbLabels();
			//std::cout << std::setw(20)<< score;
			//std::cout << std::endl;
		}
		optimize(p, score, current);
		if(score>best+1e-6){
			std::cout << std::setw(4) << ite;
			std::cout << std::setw(4) << k;
			std::cout << std::setw(4) << p.nbLabels();
			std::cout << std::setw(20)<< score;
			std::cout << std::endl;
			k=0;
			best=score;
			solution=current;
		}else{
			p.setLabels(solution);
		}
	}while(ite<1000);
	//RegisteredModularityInstance::InstancesPath = "C:/Users/manuel/Documents/GitHub/clusterisator-master/modularity_instances/";
	//for(size_t i(0); i<AvailableModularityInstancesSize; ++i){
	//	AvailableModularityInstances id(static_cast<AvailableModularityInstances>(i));
	//	RegisteredModularityInstance instance(id);
	//	instance.exportAmpl(RegisteredModularityInstance::InstancesPath +instance.name+".dat");
	//}
	system("pause");
	return 0;
}