/*
 * KMInstance.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef KM_INSTANCE_HPP_
#define KM_INSTANCE_HPP_

#include "../src/common.h"
#include "../src/RectMatrix.hpp"
#include "../src/KMConstraints.hpp"

class Agregations {
public:
	std::vector<IntSet> v;
	IntVector newIds;
};
class KMInstance;

std::ostream & operator<<(std::ostream &, KMInstance const &);
class KMInstance {
public:
	void readData(std::string const &);
	void readConstraints(std::string const &);

	bool feasible(IPartition const &) const;

	RectMatrix & data();
	RectMatrix const & data() const;
	size_t nbObs() const;
	size_t nbAtt() const;
	Double get(size_t, size_t) const;

	Double cst() const;

	DoubleVector & weights();
	DoubleVector const & weights() const;
	Double weight(size_t) const;
	Double & weight(size_t);

	void mustLink(size_t i, size_t j);
	void cannotLink(size_t i, size_t j);
	void buildMustLink(Agregations &) const;
	void allocate(size_t n, size_t m);
	/*
	 * on propage (i,j) (j,k) --> i,k
	 */
	void propagateMust();

	KMConstraints const & mustLinks() const;
	KMConstraints const & cannotLinks() const;

	KMConstraints & mustLinks();
	KMConstraints & cannotLinks();

	KMInstance();
	KMInstance(KMInstance const &, Agregations const &);

	void cpp(std::ostream &) const;
public:
	RectMatrix _data;
	Double _cst;
	DoubleVector _weights;

	KMConstraints _must;
	KMConstraints _cannot;
};

#endif /* RAWDATA_HPP_ */
