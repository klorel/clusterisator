#ifndef COLUMN_HPP
#define COLUMN_HPP

#include "gencol.h"
#include "BipartiteGraph.hpp"

class Column : public  std::pair<std::set<size_t>, std::set<size_t> > {
public:
	Double &  cost();
	Double const &  cost()const ;
	Double &  reducedCost();
	Double const &  reducedCost()const ;

	void computeCost();
	Column(BipartiteGraph const & );
	~Column();
	void addElement(size_t );
	size_t size()const;
private:
	Double _cost;
	Double _reducedCost;
	BipartiteGraph const & _input;
};


class Predicate{
public:
	bool operator()(Column const * const p, Column const * const q)const{
		return (*p)<(*q);
	}
};

#endif 

