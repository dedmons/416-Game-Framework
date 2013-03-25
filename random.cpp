#include <ctime>
#include "random.h"

Random::Random() {
  srand( time(0) );
}

float Random::getRand(float min, float max){
	//LO + (float)rand()/((float)RAND_MAX/(HI-LO));
  return min + (float)rand() / ((float)std::numeric_limits<int>::max() / ( max-min ));
}

float Random::getRand(float max){
  return ( rand() / (std::numeric_limits<int>::max()+1.0f) ) * max;
}
