#ifndef NUMBER_H
#define NUMBER_H

#include "src/common.h"
#include <random>

class Number {
public:

	static long int Generator();
	static void SetSeed(size_t);
private:
	static size_t _SEED;

#if __LP64__
	static std::mt19937_64 _Generator;
#else
	static std::mt19937 generator;
#endif

//	std::default_random_engine generator;
	static std::uniform_int_distribution<size_t> _Distribution;
};

inline long int Number::Generator() {
//	long int const i(lrand48());
	//	std::cout << i << "\n";
//	return i;
	return _Distribution(_Generator);
}

inline void Number::SetSeed(size_t seed) {
	_SEED = seed;
	_Generator.seed(seed);
//	srand48(_SEED);
}

#endif /* NUMBER_H */
