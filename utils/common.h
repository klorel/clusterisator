/*
 * common.h
 *
 *  Created on: 21 mai 2011
 *      Author: manuel
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <algorithm>
#include <numeric>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <string>

#include <cassert>

typedef std::pair<size_t, size_t> Position;
typedef std::vector<bool> BoolVector;

typedef std::set<size_t> IntSet;
typedef std::list<size_t> IntList;
typedef std::vector<size_t> IntVector;

typedef std::map<size_t, size_t> IntMap;
typedef std::set<double> DoubleSet;
typedef std::list<double> DoubleList;
typedef std::vector<double> DoubleVector;
typedef std::vector<std::string> StringVector;

typedef std::vector<IntVector> IntMatrix;
typedef std::vector<IntList> IntListVector;
typedef std::map<size_t, double> Int2Double;

template<class T> inline void DisplayContainer(std::ostream & stream,
		T const & t) {
	std::copy(t.begin(), t.end(),
			std::ostream_iterator<typename T::value_type>(stream, " "));
}

template<class T, class U> void Insert(T const & t, U & u) {
	for (auto const & it : t)
		u.insert(it);
}

IntVector SortLocation(IntVector const &location);

void Random(IntVector &);
IntVector Random(size_t const & n);

// forward declaration of interfaces
class IGraph;
class IPartition;
class IExtendedPartition;
class ICriterion;
class IMesure;
class IAlgo;
class INeighborhood;



// forward declaration of implementations
class Graph;
class Data;
class Number;
class IndexedList;
class Data;
class Density;
class Modularity;
class MultiStart;
class Vnds;

class ICriterion;
class INeighborhood;

std::ostream & operator<<(std::ostream &out, Graph const&);

#define TRACE(x) std::cout << x
#define TRACE_N(x) std::cout <<std::setw(30)<<#x<<" = "<<std::setw(10)<<x<<std::endl

#endif /* COMMON_H_ */
