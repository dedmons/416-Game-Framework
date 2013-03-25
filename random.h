#ifndef RANDOM__H
#define RANDOM__H

#include <cstdlib>
#include <limits>

class Random {
  public:
    static Random& getInstance() {
      static Random random;
      return random;
    }
    float getRand(float min, float max);
    float getRand(float max);
  private:
    Random();
    Random(const Random&);
    Random& operator=(const Random&);
};

#endif
