#include <ctime>
#include "random.h"

Random::Random() {
  srand( time(0) );
}

float Random::getRand(int min, int max){
  return min + ( rand() / (std::numeric_limits<int>::max()+1.0f) ) * ( max-min );
}

float Random::getRand(int max){
  return ( rand() / (std::numeric_limits<int>::max()+1.0f) ) * max;
}
