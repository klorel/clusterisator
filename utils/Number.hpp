#ifndef NUMBER_H
#define NUMBER_H

#include "../utils/common.h"

class Number {
public:

	static long int Generator();
	static void SetSeed(long int const &);
private:
	static long int _SEED;
};

inline long int Number::Generator() {
	long int i(lrand48());
//	std::cout << i << "\n";
	return i;
}

inline void Number::SetSeed(long int const & seed) {
	_SEED = seed;
	srand48(_SEED);
}

#endif /* NUMBER_H */
