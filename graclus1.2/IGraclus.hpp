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
	typedef std::vector<std::pair<unsigned int, double> > Row;
	static IGraclus * Get();
public:
	virtual void allocate(unsigned const int &, unsigned const int &,
			bool const &) = 0;
	virtual void setRow(unsigned const int &, Row const &)= 0;
	virtual void check()= 0;

	virtual void launch(int nparts) = 0;
	virtual unsigned int get(unsigned const int &) const=0;
	virtual double score() const = 0;
	virtual ~IGraclus();
};

#endif /* IGRACLUS_HPP_ */
