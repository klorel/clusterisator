/*
 * common.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "src/common.h"
#include "Number.hpp"

IntVector SortLocation(IntVector const &location) {
	IntVector ret(location.size());
	IntMap map;
	for (size_t i(0); i < location.size(); ++i) {
		IntMap::const_iterator ite(map.find(location[i]));
		if (ite == map.end()) {
			size_t newLabel(map.size());
			map.insert(IntMap::value_type(location[i], newLabel));
		}
	}
	for (size_t i(0); i < location.size(); ++i) {
		size_t newLabel(map[location[i]]);
		ret[i] = newLabel;
	}
	return ret;
}

void Random(IntVector & v) {
	for (auto & it : v)
		it = Number::Generator() % v.size();

}
IntVector Random(size_t n) {
	IntVector v(n);
	Random(v);
	return v;
}

size_t PopRandom(IntVector & v, size_t & n) {
	std::swap(v.back(), v[Number::Generator() % n]);
	--n;
	return v.back();
}

