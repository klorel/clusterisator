/*
 * VnsLabelPropagation.cpp
 *
 *  Created on: 12 mars 2014
 *      Author: manuel
 */

#include "VnsLabelPropagation.h"

#include "LabelPropagation.h"

namespace bipartite {
VnsLabelPropagation::VnsLabelPropagation(BipartiteGraph & instance,
                                         ModularityBPartition & solution,
                                         int kMax, int iteMax)
    : _instance(instance),
      _solution(solution),
      _kMax(kMax),
      _iteMax(iteMax) {
  // TODO Auto-generated constructor stub

}

VnsLabelPropagation::~VnsLabelPropagation() {
  // TODO Auto-generated destructor stub
}

void VnsLabelPropagation::singletize(int i, IndexedList & allNodes,
                                     ModularityBPartition & solution) {
  allNodes.fill();
  for (int p(0); p < i && !allNodes.empty(); ++p) {
    int const id(allNodes.pop_random());
    if (solution.sizeOfLabel(solution.label(id)) > 1) {
      solution.shift(id, solution.getUnUsedLabel());
      ++p;
    }
  }
  solution.score() = solution.computeScore();
}

void VnsLabelPropagation::set(IntVector const & l,
                              ModularityBPartition & solution) {
  for (int i(0); i < l.size(); ++i)
    solution.shift(i, l[i]);
}

void VnsLabelPropagation::run() {
  MY_PRINT(_kMax);
  MY_PRINT(_instance.nV());
  LabelPropagation lpa(_instance, _solution);

  IndexedList allNodes(_instance.nV(), true);
  IntVector best(_solution.labels());
  Double bestScore(_solution.score());
  int ite(0);
  std::cout << std::setw(4) << ite;
  std::cout << std::setw(4) << 0;
  std::cout << std::setw(4) << _solution.nbLabels();
  std::cout << std::setw(15) << _solution.score();
  std::cout << std::endl;
  do {
    int k(0);
    ++ite;
    do {
      ++k;
      singletize(k, allNodes, _solution);
      lpa();
      if (bestScore + 1e-10 < _solution.score()) {
        bestScore = _solution.score();
        best = _solution.labels();
        std::cout << std::setw(4) << ite;
        std::cout << std::setw(4) << k;
        std::cout << std::setw(4) << _solution.nbLabels();
        std::cout << std::setw(15) << _solution.score();
        std::cout << std::endl;
        k = 0;
      } else {
        set(best, _solution);
      }

    } while (k <= _kMax);
  } while (ite <= _iteMax);
  set(best, _solution);
}
}
