#include "Number.hpp"
size_t Number::_SEED = 0;

#if __LP64__
std::mt19937_64 Number::_Generator = std::mt19937_64(0);
#else
std::mt19937 Number::_Generator=std::mt19937 (0);
#endif

//	std::default_random_engine generator;
std::uniform_int_distribution<size_t> Number::_Distribution =
		std::uniform_int_distribution < size_t
				> (0, std::numeric_limits<size_t>::max());
