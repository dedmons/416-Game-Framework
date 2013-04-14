#include <ctime>
#include <iostream>
#include "random.h"

Random::Random() {
  srand( time(0) );
}

float Random::getRand(float min, float max){
	// std::cout << "Getting rand btwn " << min << " and " << max << std::endl;
  float retval = min + (rand() / (std::numeric_limits<int>::max()+1.0f)) * ( max-min );
  // std::cout << "Value: " << retval << std::endl;
  return retval;
}

float Random::getRand(float max){
  return ( rand() / (std::numeric_limits<int>::max()+1.0f) ) * max;
}
