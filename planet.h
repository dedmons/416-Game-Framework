#ifndef PLANET__H
#define PLANET__H
#include <string>

#include "jsongamedata.h"
#include "vector2f.h"
#include "drawable.h"

class Planet : public Drawable{
public:
  Planet(const std::string&, float, float, Vector2f);
  Planet(const Planet&);

  const Frame* getFrame() const { return frame; }
  void draw() const;
  void update(Uint32 ticks){ ++ticks; };

  float getMass() const { return mass; }
  float getRadius() const { return radius; }
  Vector2f getLocation() const;

private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;

  float mass;
  float radius;

  Planet();
};
#endif
