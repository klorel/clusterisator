/*
 * common.h
 *
 *  Created on: 21 mai 2011
 *      Author: manuel
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "typedef.h"

std::ostream & operator<<(std::ostream &out, Graph const&);

template<class T> inline void DisplayContainer(std::ostream & stream,
                                               T const & t, char const * str =
                                                   " ") {
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
template<class T, class U> inline void PushBack(T const & t, U & u, int n) {
  auto it(t.begin());
  int i(0);
  while (it != t.end() && i < n) {
    u.push_back(*it);
    ++i;
  }
}

IntVector SortLocation(IntVector const &location);

void Random(IntVector &);
int RandomElement(IntVector const &);
IntVector Random(int n);

int PopRandom(IntVector &, int & n);

template<class T> inline bool IsEqual(T const & t1, T const & t2) {
  return std::abs(t1 - t2) <= 1e-6;
}
template<> inline bool IsEqual(Double const & t1, Double const & t2) {
  Double const bigger(std::max(std::abs(t1), std::abs(t2)));
  return std::abs(t1 - t2) / (bigger > 1 ? bigger : 1) <= 1e-6;
}

template<> inline bool IsEqual<DoubleVector>(DoubleVector const & t1,
                                             DoubleVector const & t2) {
  for (int i(0); i < t1.size(); ++i)
    if (!IsEqual(t1[i], t2[i]))
      return false;
  return true;
}

void operator>>(std::istream &, IntVector &);

template<class T1> std::string GetStr(T1 const & t1) {
  std::stringstream stream;
  stream << t1;
  std::string result(stream.str());
  return result;
}

template<class T1, class T2> std::string GetStr(T1 const & t1, T2 const & t2) {
  std::stringstream stream;
  stream << t1 << t2;
  std::string result(stream.str());
  return result;
}

template<class T1, class T2, class T3> std::string GetStr(T1 const & t1,
                                                          T2 const & t2,
                                                          T3 const & t3) {
  std::stringstream stream;
  stream << t1 << t2 << t3;
  std::string result(stream.str());
  return result;
}

template<class T1, class T2, class T3, class T4> std::string GetStr(
    T1 const & t1, T2 const & t2, T3 const & t3, T4 const & t4) {
  std::stringstream stream;
  stream << t1 << t2 << t3 << t4;
  std::string result(stream.str());
  return result;
}
template<class T1, class T2, class T3, class T4, class T5>
inline std::string GetStr(T1 const &t1, T2 const & t2, T3 const & t3,
                          T4 const & t4, T5 const & t5) {
  std::stringstream stream;
  stream << t1 << t2 << t3 << t4 << t5;
  std::string result(stream.str());
  return result;
}
template<class T1, class T2, class T3, class T4, class T5, class T6>
inline std::string GetStr(T1 const &t1, T2 const & t2, T3 const & t3,
                          T4 const & t4, T5 const & t5, T6 const & t6) {
  std::stringstream stream;
  stream << t1 << t2 << t3 << t4 << t5 << t6;
  std::string result(stream.str());
  return result;
}
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
inline std::string GetStr(T1 const & t1, T2 const & t2, T3 const & t3,
                          T4 const & t4, T5 const & t5, T6 const & t6,
                          T7 const & t7) {
  std::stringstream stream;
  stream << t1 << t2 << t3 << t4 << t5 << t6 << t7;
  std::string result(stream.str());
  return result;
}
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7,
    class T8>
inline std::string GetStr(T1 const & t1, T2 const & t2, T3 const & t3,
                          T4 const & t4, T5 const & t5, T6 const & t6,
                          T7 const & t7, T8 const & t8) {
  std::stringstream stream;
  stream << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8;
  std::string result(stream.str());
  return result;
}
template<class T1, class T2, class T3, class T4, class T5, class T6, class T7,
    class T8, class T9>
inline std::string GetStr(T1 const & t1, T2 const & t2, T3 const & t3,
                          T4 const & t4, T5 const & t5, T6 const & t6,
                          T7 const & t7, T8 const & t8, T9 const & t9) {
  std::stringstream stream;
  stream << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9;
  std::string result(stream.str());
  return result;
}

#define MY_PRINT(x) std::cout << #x << " = "<<x<<std::endl

#define __SEG_FAULT__ {std::vector<int> __toto__;__toto__[10]=1;}

inline int ijtok(int n, int i, int j) {
  if (i == j) {
    std::cout << "error " << i << " = " << j << std::endl;
    std::exit(-1);
  }
  int const l(std::min(i, j));
  int const u(std::max(i, j));
  return (2 * n - l - 1) * l / 2 + u - l - 1;
}

inline double checkCoeff(double v) {
  return std::fabs(v) < 1e-20 ? 0 : v;
}

//#define ASSERT_NON_ZERO(x) assert(x>0 && "THIS VALUE MUST BE STRICLY POSITIVE" );
//#define ASSERT_NON_ZERO(x)
#endif /* COMMON_H_ */
