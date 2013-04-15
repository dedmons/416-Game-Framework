#include <vector>
#include <algorithm>
#include "collisionStrategy.h"

using std::cout; using std::endl;

bool RectangularCollisionStrategy::execute(
      const Drawable& obj1, const Drawable& obj2) const {
  float left1 = obj1.X();
  float left2 = obj2.X();
  float right1 = left1+obj1.getFrame()->getWidth();
  float right2 = left2+obj2.getFrame()->getWidth();
  float top1 = obj1.Y();
  float top2 = obj2.Y();
  float bottom1 = top1+obj1.getFrame()->getHeight();
  float bottom2 = top2+obj2.getFrame()->getHeight();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;
  return true;
}

float MidPointCollisionStrategy::
distance(float x1, float y1, float x2, float y2) const {
  float x = x1-x2, y = y1-y2;
  return hypot(x, y);
}

bool MidPointCollisionStrategy::execute(
      const Drawable& obj1, const Drawable& obj2) const {
  int COLLISION_DISTANCE = 
    obj1.getFrame()->getWidth()/2 + obj2.getFrame()->getWidth()/2;
  float xobj1 = obj1.X()+obj1.getFrame()->getWidth()/2;
  float yobj1 = obj1.Y()+obj1.getFrame()->getHeight()/2;
  float xobj2 = obj2.X() + obj2.getFrame()->getWidth()/2;
  float yobj2 = obj2.Y() + obj2.getFrame()->getHeight()/2;
  return distance(xobj1, yobj1, xobj2, yobj2) < COLLISION_DISTANCE;
}

