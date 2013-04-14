#ifndef PLANETS__H
#define PLANETS__H
#include <string>
#include <map>

#include "jsongamedata.h"
#include "vector2f.h"
#include "planet.h"

class Planets {
public:
  static Planets& getInstance();

  void draw();

  Vector2f accelerationFromPlanets(Vector2f loc);

private:
  std::vector<Planet> planets;

  Planets();
  Planets(const Planets&);
  Planets& operator=(const Planets&);
};
#endif
