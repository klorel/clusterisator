/*
 * LabelPropagation2.cpp
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#include "LabelPropagation2.h"

LabelPropagation2::LabelPropagation2(ICliquePartition & solution)
    : _input(solution.problem()),
      _solution(solution) {
  _input.adjencyGraph(_graph);
}

LabelPropagation2::~LabelPropagation2() {

}

bool LabelPropagation2::operator()(int id) {
  std::map<int, Double> links;
  for (auto const & v : _graph[id]) {
    links[_solution.label(v.first)] += v.second;
  }
  Double const loss(links[_solution.label(id)]);
  std::multimap<Double, int, std::greater<Double>> delta;
  for (auto const & n : links) {
    delta.insert(std::make_pair(n.second - loss, n.first));
  }
  bool success(delta.begin()->first > 1e-6);
  if (success) {
    _solution.checkScore();
    _solution.shift(id, delta.begin()->second);
    _solution.score() += delta.begin()->first;
    _solution.checkScore();
  }
  return success;
}
bool LabelPropagation2::operator()() {
  bool success(false);
  bool stop(true);
  _solution.checkScore();
  do {
    stop = true;
    for (int id(0); id < _input.nV(); ++id) {
      if (operator ()(id)) {
        stop = false;
        success = true;
      }
    }
  } while (!stop);
  _solution.checkScore();
  return success;
}
