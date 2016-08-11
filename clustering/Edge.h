/*
 * Edge.h
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef EDGE_HPP_
#define EDGE_HPP_

#include "common.h"

class Edge {
 public:
  int _i;
  int _j;
  Double _v;
  Edge(int i = -1, int j = -1, Double v = 1)
      : _i(i),
        _j(j),
        _v(v) {
  }
  bool operator<(Edge const & rhs) const {
    return std::less<std::pair<int, int>>()(std::make_pair(_i, _j),
                                            std::make_pair(rhs._i, rhs._j));
  }
};

class Edges : public std::list<Edge> {
 public:
  bool read(std::string const &, std::ostream & = std::cout);
  void print(std::ostream &) const;
 public:
  Edges();
  virtual ~Edges();

};

std::ostream & operator<<(std::ostream & stream, Edges const & rhs);
#endif /* EDGE_HPP_ */
