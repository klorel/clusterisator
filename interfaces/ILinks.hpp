/*
 * ILinks.hpp
 *
 *  Created on: 10 août 2012
 *      Author: manuel
 */

#ifndef I_LINKS_HPP_
#define I_LINKS_HPP_

typedef std::pair<size_t, double> Link;
typedef std::vector<std::pair<size_t, double> > Links;

#include "LinksIterator.hpp"

class ILinks {
public:
	enum Kind {
		Similarity, Disimilarity, Other
	};
public:
	virtual size_t nbLinks() const = 0;
	virtual size_t nbObs() const = 0;
	// return elements linked to i
	virtual LinksIterator links(size_t i) const = 0;
	// return the links between i and j
	virtual double const & link(size_t i, size_t j) const = 0;
	// return the weight of element i
	virtual double const & weight(size_t i) const = 0;
	//
	virtual ~ILinks();
};

inline ILinks::~ILinks() {

}
#endif /* ILINH_HPP_ */
