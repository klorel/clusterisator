/*
 * IGraclus.hpp
 *
 *  Created on: 2 août 2012
 *      Author: manuel
 */

#ifndef IGRACLUS_HPP_
#define IGRACLUS_HPP_

#include <vector>
// needed for size_t declaration
#include <cstdlib>

class Graph;
class IGraclus {
public:
	typedef std::vector<std::pair<size_t, double> > Row;
	static IGraclus * Get();
public:
	virtual void allocate(size_t const&, size_t const &, bool const &) = 0;
	virtual void setRow(size_t const&, Row const &)= 0;
	virtual void check()= 0;

	virtual void launch(int nparts) = 0;
	virtual size_t get(size_t const&) const=0;
	virtual double score() const = 0;
	virtual ~IGraclus();
};

#endif /* IGRACLUS_HPP_ */
