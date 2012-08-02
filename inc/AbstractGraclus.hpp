/*
 * IGraclus.hpp
 *
 *  Created on: 2 août 2012
 *      Author: manuel
 */

#ifndef ABSTRACT_GRACLUS_HPP_
#define ABSTRACT_GRACLUS_HPP_

#include "common.h"
#include "gracluslib/metis.h"
#include "IGraclus.hpp"

class AbstractGraclus: public IGraclus {
public:
	AbstractGraclus();
	virtual ~AbstractGraclus();
public:
	void set(Graph const &);
	void launch(int nparts);
	unsigned int get(unsigned const int & vtx) const;
	double score() const;
protected:
	void readGraph(Graph const &);
	void init();
	void free();
private:

	GraphType _graph;
	IntVector _partition;
	double _score;
	int _wgtflag;
};

#endif /* ABSTRACT_GRACLUS_HPP_ */
