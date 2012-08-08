/*
 * IGraclus.hpp
 *
 *  Created on: 2 août 2012
 *      Author: manuel
 */

#ifndef ABSTRACT_GRACLUS_HPP_
#define ABSTRACT_GRACLUS_HPP_

#include "metis.h"
#include "IGraclus.hpp"

class AbstractGraclus: public IGraclus {
public:
	AbstractGraclus();
	virtual ~AbstractGraclus();
public:
	void allocate(unsigned  int const &, unsigned  int const &, bool const &);
	void setRow(unsigned const int &, Row const &);
	void check();

	void launch(int nparts);
	unsigned int get(unsigned const int & vtx) const;
	double score() const;
protected:
	void init();
	void free();
private:
	GraphType _graph;
	std::vector<unsigned  int> _partition;
	double _score;
	int _wgtflag;
	int _k;
	bool _isWeighted;
};

#endif /* ABSTRACT_GRACLUS_HPP_ */
