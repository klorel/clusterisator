/*
 * common.h
 *
 *  Created on: 21 mai 2011
 *      Author: manuel
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "typedef.h"
#include "Env.hpp"

#define OUT Env::Get()<<""

std::ostream & operator<<(std::ostream &out, Graph const&);

template<class T> inline void DisplayContainer(std::ostream & stream,
		T const & t) {
	std::copy(t.begin(), t.end(),
			std::ostream_iterator<typename T::value_type>(stream, " "));
}

template<class T, class U> inline void Insert(T const & t, U & u) {
	for (auto const & it : t)
		u.insert(it);
}

IntVector SortLocation(IntVector const &location);

void Random(IntVector &);
IntVector Random(size_t const & n);

template<class T> constexpr T Zero() {
	return static_cast<T>(0);
}

#endif /* COMMON_H_ */
