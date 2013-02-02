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
    float getRand(int min, int max);
    float getRand(int max);
  private:
    Random();
    Random(const Random&);
    Random& operator=(const Random&);
};

#endif
