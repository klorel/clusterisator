#ifndef DIVISOR_HPP
#define DIVISOR_HPP

#include "gencol.h"
#include "Column.hpp"
#include "BipartiteGraph.hpp"
#include "ModularityBPartition.hpp"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;

class Divisor {
public:
	Divisor(BipartiteGraph const &);
	Divisor(BipartiteGraph const *);
	~Divisor();
public:
	void freeLp();
	Double buildLp(ModularityBPartition const &, size_t);
	void write(std::string const & fileName = "divisor.lp") const;

	bool run(ModularityBPartition &, size_t label);

	void initCpx();

private:
	CPXENVptr _env;
	CPXLPptr _lp;

	BipartiteGraph const * _input;

	std::vector<int> _y;

};

#endif 
