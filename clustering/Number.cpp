#include "Number.hpp"
size_t Number::_SEED = 0;

std::default_random_engine Number::_Generator = std::default_random_engine(1);

//	std::default_random_engine generator;
std::uniform_int_distribution<size_t> Number::_Distribution =
		std::uniform_int_distribution < size_t
				> (0, std::numeric_limits<size_t>::max());

void Number::SetSeed(size_t seed) {
	assert(seed != 0);
	_SEED = seed;
	_Generator.seed(seed);
//	srand48(_SEED);
}
