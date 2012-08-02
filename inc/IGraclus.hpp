/*
 * IGraclus.hpp
 *
 *  Created on: 2 août 2012
 *      Author: manuel
 */

#ifndef IGRACLUS_HPP_
#define IGRACLUS_HPP_

#include <vector>

class Graph;
class IGraclus {
public:
	static IGraclus * Get();
public:
	virtual void launch(int nparts) = 0;
	virtual unsigned int get(unsigned const int & vtx) const=0;
	virtual double score() const = 0;
	virtual ~IGraclus();
protected:
	virtual void init() = 0;
	virtual void free() = 0;

public:
	virtual void set(Graph const &) = 0;

};

#endif /* IGRACLUS_HPP_ */
