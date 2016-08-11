/*
 * Decision.h
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef DECISION_HPP
#define DECISION_HPP

#include "gencol.h"
class Decision {
 public:
  Decision();
  Decision(Decision const &);
  Decision(int, int, bool);
  Decision(std::pair<int, int> noeud1noeud2, bool = false);
  Decision & operator=(Decision const &);
  virtual ~Decision();
  bool cannot() const;
  bool &cannot();
  int noeud1() const;
  int &noeud1();
  int noeud2() const;
  int &noeud2();

  bool operator<(Decision const &) const;
  void print(std::ostream & = std::cout) const;
  std::string name() const;

  static void Print(DecisionList const &, std::ostream & = std::cout);
  static void Print(DecisionSet const &, std::ostream & = std::cout);

  int violation(bool, bool) const;
 private:
  int _noeud1;
  int _noeud2;
  bool _cannot;
  bool _empty;
};

#endif /* DECISION_H_ */
