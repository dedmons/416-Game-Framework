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
    int getRandInt(int min, int max);
    int getRandInt(int max);
  private:
    Random();
    Random(const Random&);
    Random& operator=(const Random&);
};

Random::Random() {
  srand(time(NULL));
}

#endif
