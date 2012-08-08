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
	virtual void allocate(unsigned int const&, unsigned int const &,
			bool const &) = 0;
	virtual void setRow(unsigned int const&, Row const &)= 0;
	virtual void check()= 0;

	virtual void launch(int nparts) = 0;
	virtual unsigned int get(unsigned int const&) const=0;
	virtual double score() const = 0;
	virtual ~IGraclus();
};

#endif /* IGRACLUS_HPP_ */
