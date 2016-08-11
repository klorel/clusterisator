/*
 * common.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "common.h"

#include "Number.h"

IntVector SortLocation(IntVector const &location) {
  IntVector ret(location.size());
  IntMap map;
  for (int i(0); i < location.size(); ++i) {
    IntMap::const_iterator ite(map.find(location[i]));
    if (ite == map.end()) {
      int newLabel((int) map.size());
      map.insert(IntMap::value_type(location[i], newLabel));
    }
  }
  for (int i(0); i < location.size(); ++i) {
    int newLabel(map[location[i]]);
    ret[i] = newLabel;
  }
  return ret;
}

void Random(IntVector & v) {
  for (auto & it : v)
    it = Number::Generator() % v.size();

}
IntVector Random(int n) {
  IntVector v(n);
  Random(v);
  return v;
}

int PopRandom(IntVector & v, int & n) {
  std::swap(v.back(), v[Number::Generator() % n]);
  --n;
  return v.back();
}

int RandomElement(IntVector const & v) {
  return v[Number::Generator() % v.size()];
}

