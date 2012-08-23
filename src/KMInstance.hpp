/*
 * KMInstance.hpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#ifndef KM_INSTANCE_HPP_
#define KM_INSTANCE_HPP_

#include "src/common.h"
#include "src/RectMatrix.hpp"
#include "src/KMConstraints.hpp"

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

inline RectMatrix const & KMInstance::data() const {
	return _data;
}
inline RectMatrix & KMInstance::data() {
	return _data;
}
inline size_t KMInstance::nbObs() const {
	return _data.getN();
}
inline size_t KMInstance::nbAtt() const {
	return _data.getM();
}

inline Double KMInstance::cst() const {
	return _cst;
}
inline Double KMInstance::get(size_t i, size_t j) const {
	return _data.get(i, j);
}
inline Double KMInstance::weight(size_t i) const {
	return _weights[i];
}
inline Double & KMInstance::weight(size_t i) {
	return _weights[i];
}

inline DoubleVector const & KMInstance::weights() const {
	return _weights;
}

inline DoubleVector & KMInstance::weights() {
	return _weights;
}

inline void KMInstance::mustLink(size_t i, size_t j) {
	_must.newCtr(i, j);
}
inline void KMInstance::cannotLink(size_t i, size_t j) {
	_cannot.newCtr(i, j);
}
inline KMConstraints const & KMInstance::mustLinks() const {
	return _must;
}
inline KMConstraints const & KMInstance::cannotLinks() const {
	return _cannot;
}
inline KMConstraints & KMInstance::mustLinks() {
	return _must;
}
inline KMConstraints & KMInstance::cannotLinks() {
	return _cannot;
}

#endif /* RAWDATA_HPP_ */
