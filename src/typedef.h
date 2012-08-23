/*
 * typedef.h
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

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

#undef NDEBUG
#include <cassert>

#if __LP64__
typedef long double Double;
#else
typedef double Double;
#endif

typedef std::pair<size_t, size_t> Position;
typedef std::vector<bool> BoolVector;

typedef std::set<size_t> IntSet;
typedef std::list<size_t> IntList;
typedef std::vector<size_t> IntVector;

typedef std::map<size_t, size_t> IntMap;
typedef std::set<Double> DoubleSet;
typedef std::list<Double> DoubleList;
typedef std::vector<Double> DoubleVector;
typedef std::vector<std::string> StringVector;

typedef std::vector<IntVector> IntMatrix;
typedef std::vector<DoubleVector> DoubleMatrix;

typedef std::vector<IntList> IntListVector;
typedef std::map<size_t, Double> Int2Double;

typedef std::pair<Double, Double> Double2;

typedef std::multimap<Double, size_t, std::greater<Double> > Distances;
typedef std::pair<size_t, size_t> Move;
typedef std::vector<Move> Moves;

// forward declaration of interfaces
class ILinks;
class IPartition;
class ISolution;
class ICriterion;
class IMesure;
class IAlgo;
class INeighborhood;
class Problem;
class ISolution;

// forward declaration of implementations
class Graph;
class Solution;
class Number;
class IndexedList;
class Solution;
class Density;
class Modularity;
class MultiStart;
class Vnds;

class ICriterion;
class INeighborhood;

#endif /* TYPEDEF_H_ */
