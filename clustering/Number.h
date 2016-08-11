#ifndef NUMBER_H
#define NUMBER_H

#include "common.h"
#include <random>

class Number {
 public:

  static int Generator();
  static void SetSeed(int);
 private:
  static int _SEED;

  static std::default_random_engine _Generator;
//	std::default_random_engine generator;
  static std::uniform_int_distribution<int> _Distribution;
};

inline int Number::Generator() {
//	long int const i(lrand48());
  //	std::cout << i << "\n";
//	return i;
  return _Distribution(_Generator);
}

#endif /* NUMBER_H */
