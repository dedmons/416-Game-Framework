#ifndef __WORLD__H
#define __WORLD__H

#include "frame.h"
#include "viewport.h"
#include "jsongamedata.h"

class World {
public:
  World(Frame* const frame, float fact = 1) :
      frame1(*frame),
      frameWidth( frame->getWidth() ),
      frameHeight( frame->getHeight() ),
      worldWidth( JSONGamedata::getInstance().getInt("view.width") ),
      worldHeight( JSONGamedata::getInstance().getInt("view.height") ),
      factor(fact),
      viewX(0.0), viewY(0.0),
      view(Viewport::getInstance())
  {}
  World(const World& other) :
      frame1( other.frame1 ),
      frameWidth( other.frameWidth ),
      frameHeight( other.frameHeight ),
      worldWidth( other.worldWidth ),
      worldHeight( other.worldHeight ),
      factor( other.factor ),
      viewX( other.viewX ), viewY( other.viewY ),
      view( other.view )
  {}
  void update();
  void draw() const;
  World& operator=(const World&);

private:
  Frame frame1;
  unsigned frameWidth;
  unsigned frameHeight;
  unsigned worldWidth;
  unsigned worldHeight;
  float factor;
  float viewX;
  float viewY;
  Viewport & view;
};

#endif
