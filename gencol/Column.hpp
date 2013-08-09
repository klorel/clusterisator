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

	Double computeCost()const;
	Double computeReducedCost(DoubleVector const & dual)const;
	Column(BipartiteGraph const & );
	~Column();
	void addElement(size_t );
	size_t size()const;
	size_t & id() const;
	void check(DoubleVector const &) const ;
	void print(std::ostream & = std::cout)const;
private:
	Double _cost;
	Double _reducedCost;
	BipartiteGraph const & _input;
	mutable size_t _id;
};


class Predicate{
public:
	bool operator()(Column const * const p, Column const * const q)const{
		return (*p)<(*q);
	}
};

#endif 

