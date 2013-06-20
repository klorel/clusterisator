#ifndef COLUMN_HPP
#define COLUMN_HPP
#include "gencol.h"

class Column : public  std::set<size_t> {
public:
	Double &  cost();
	Double const &  cost()const ;
	Double &  reducedCost();
	Double const &  reducedCost()const ;

private:
	double _cost;
	double _reducedCost;
	Graph const & _input;
};


class Predicate{
public:
	bool operator()(Column const * const p, Column const * const q)const{
		return (*p)<(*q);
	}
};

#endif 
