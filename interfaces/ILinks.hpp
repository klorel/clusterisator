/*
 * ILinks.hpp
 *
 *  Created on: 10 août 2012
 *      Author: manuel
 */

#ifndef I_LINKS_HPP_
#define I_LINKS_HPP_

#include "../utils/common.h"

typedef std::pair<size_t, Double> Link;
typedef std::vector<std::pair<size_t, Double> > Links;

#include "../data/LinksIterator.hpp"

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
	virtual Double link(size_t i, size_t j) const = 0;
	// return the weight of element i
	virtual Double weight(size_t i) const = 0;
	// return the weight of element i
	virtual Double sumOfNodeLinks(size_t i) const = 0;
	//
	virtual ~ILinks();
};

inline ILinks::~ILinks() {

}
#endif /* ILINH_HPP_ */
