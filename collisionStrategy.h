#ifndef COLLISIONSTRAT__H
#define COLLISIONSTRAT__H
#include <cmath>
#include "drawable.h"

class CollisionStrategy {
public:
  virtual bool execute(const Drawable&, const Drawable&) const = 0;
  virtual ~CollisionStrategy() {}
};

class RectangularCollisionStrategy : public CollisionStrategy {
public:
  RectangularCollisionStrategy() {}
  virtual bool execute(const Drawable&, const Drawable&) const;
};

class MidPointCollisionStrategy : public CollisionStrategy {
public:
  MidPointCollisionStrategy() {}
  virtual bool execute(const Drawable&, const Drawable&) const;
  float distance(float, float, float, float) const;
};

class CollisionTest {
public:
	enum TestType {Rectangular,MidPoint,Pixel};

	static CollisionTest& getInstance();
	bool check(TestType, const Drawable&, const Drawable&) const;
};

#endif
