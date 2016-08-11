#include "Number.h"
int Number::_SEED = 0;

std::default_random_engine Number::_Generator = std::default_random_engine(1);

//	std::default_random_engine generator;
std::uniform_int_distribution<int> Number::_Distribution =
    std::uniform_int_distribution<int>(0, std::numeric_limits<int>::max());

void Number::SetSeed(int seed) {
  assert(seed != 0);
  _SEED = seed;
  _Generator.seed(seed);
  //	srand48(_SEED);
}
