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

	static std::default_random_engine _Generator;
//	std::default_random_engine generator;
	static std::uniform_int_distribution<size_t> _Distribution;
};

inline long int Number::Generator() {
//	long int const i(lrand48());
	//	std::cout << i << "\n";
//	return i;
	return _Distribution(_Generator);
}


#endif /* NUMBER_H */
