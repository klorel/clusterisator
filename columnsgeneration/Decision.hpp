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
	Decision(size_t, size_t, bool);
	Decision(std::pair<size_t, size_t> noeud1noeud2, bool = false);
	Decision & operator=(Decision const &);
	virtual ~Decision();
	bool cannot() const;
	bool &cannot();
	size_t noeud1() const;
	size_t &noeud1();
	size_t noeud2() const;
	size_t &noeud2();

	bool operator<(Decision const &) const;
	void print(std::ostream & = std::cout) const;
	std::string name() const;

	static void Print(DecisionList const &, std::ostream & = std::cout);
	static void Print(DecisionSet const &, std::ostream & = std::cout);

	size_t violation(bool, bool) const;
private:
	size_t _noeud1;
	size_t _noeud2;
	bool _cannot;
	bool _empty;
};

#endif /* DECISION_H_ */
