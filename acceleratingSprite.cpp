#include <cmath>
#include "acceleratingSprite.h"
#include "jsongamedata.h"

// AcceleratingSprite::AcceleratingSprite(const std::string& name, const Frame* fm) :
//   Sprite(name,fm),
//   acceleration(
//       (rand()%2?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.x"),
//       (rand()%2?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.y")
//   ),
//   maxSpeeds(
//       JSONGamedata::getInstance().getInt(name+".speed.max.x"),
//       JSONGamedata::getInstance().getInt(name+".speed.max.y")
//   )
// { }

AcceleratingSprite::AcceleratingSprite(const std::string& name, const int scaleMin, const int scaleMax) :
  Sprite(name,scaleMin,scaleMax),
  acceleration(
      (rand()%2?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.x"),
      (rand()%2?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.y")
  ),
  maxSpeeds(
      JSONGamedata::getInstance().getInt(name+".speed.max.x"),
      JSONGamedata::getInstance().getInt(name+".speed.max.y")
  )
{ }

AcceleratingSprite::AcceleratingSprite(const AcceleratingSprite& o) :
  Sprite(o.getName(), o.getScale()),
  acceleration(o.acceleration),
  maxSpeeds(o.maxSpeeds)
{ }

/*
Sprite& Sprite::operator=(const Sprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  frame = rhs.frame;
  acceleration = rhs.acceleration;
  maxSpeeds = rhs.maxSpeeds;
  return *this;
}
*/

void AcceleratingSprite::updateVelocity(Uint32 ticks){

  float inc = acceleration[0] * 0.001 * static_cast<float>(ticks);
  velocityX( velocityX() + inc );

  inc = acceleration[1] * 0.001 * static_cast<float>(ticks);
  velocityY( velocityY() + inc );

  if(velocityX() > maxSpeeds[0]) {
    velocityX( maxSpeeds[0] );
  }

  if(velocityY() > maxSpeeds[1]) {
    velocityY( maxSpeeds[1] );
  }
}


void AcceleratingSprite::update(Uint32 ticks) {
  updateVelocity(ticks);
  Sprite::update(ticks);
}
