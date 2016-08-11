#ifndef DIVISOR_HPP
#define DIVISOR_HPP

#include "../bipartite_data/BipartiteGraph.h"
#include "../bipartite_data/ModularityBPartition.h"
#include "../columnsgeneration/Column.h"
#include "gencol.h"

typedef struct cpxlp* CPXLPptr;
typedef struct cpxenv* CPXENVptr;

namespace bipartite {
class Divisor {
 public:
  Divisor(BipartiteGraph const &);
  Divisor(BipartiteGraph const *);
  ~Divisor();
 public:
  void freeLp();
  Double buildLp(ModularityBPartition const &, int);
  void write(std::string const & fileName = "divisor.lp") const;

  bool run(ModularityBPartition &, int label);

  void initCpx();

 private:
  CPXENVptr _env;
  CPXLPptr _lp;

  BipartiteGraph const * _input;

  std::vector<int> _y;

};
}
#endif 
