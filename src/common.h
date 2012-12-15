/*
 * common.h
 *
 *  Created on: 21 mai 2011
 *      Author: manuel
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "typedef.h"

#define OUT std::cout

std::ostream & operator<<(std::ostream &out, Graph const&);

template<class T> inline void DisplayContainer(std::ostream & stream,
		T const & t, char const * str = " ") {
	std::copy(t.begin(), t.end(),
			std::ostream_iterator<typename T::value_type>(stream, str));
	stream << "\n";
}

template<class T, class U> inline void Insert(T const & t, U & u) {
	for (auto const & it : t)
		u.insert(it);
}
template<class T, class U> inline void PushBack(T const & t, U & u) {
	for (auto const & it : t)
		u.push_back(it);
}
template<class T, class U> inline void PushBack(T const & t, U & u, size_t n) {
	auto it(t.begin());
	size_t i(0);
	while (it != t.end() && i < n) {
		u.push_back(*it);
		++i;
	}
}

IntVector SortLocation(IntVector const &location);

void Random(IntVector &);
IntVector Random(size_t n);

size_t PopRandom(IntVector &, size_t & n);


template<class T> inline bool IsEqual(T const & t1, T const & t2) {
	return std::abs(t1 - t2) <= 1e-6;
}
template<> inline bool IsEqual(Double const & t1, Double const & t2) {
	Double const bigger(std::max(std::abs(t1), std::abs(t2)));
	return std::abs(t1 - t2) / (bigger > 1 ? bigger : 1)
			<= 1e-6;
}

template<> inline bool IsEqual<DoubleVector>(DoubleVector const & t1,
		DoubleVector const & t2) {
	for (size_t i(0); i < t1.size(); ++i)
		if (!IsEqual(t1[i], t2[i]))
			return false;
	return true;
}

void operator>>(std::istream &, IntVector &);
#endif /* COMMON_H_ */
