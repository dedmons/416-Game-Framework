#include <cmath>
#include "acceleratingSprite.h"

Sprite::Sprite(const std::string& name, const Frame* fm) :
  Drawable(name,
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"),
                    Gamedata::getInstance()->getXmlInt(name+"Y")),
           Vector2f(
             (rand()%2?1:-1)*Random::getInstance().getRand(
                Gamedata::getInstance()->getXmlInt(name+"SpeedXMin"),
                Gamedata::getInstance()->getXmlInt(name+"StartSpeedSeed")),
             (rand()%2?1:-1)*Random::getInstance().getRand(
                  Gamedata::getInstance()->getXmlInt(name+"SpeedYMin"),
                  Gamedata::getInstance()->getXmlInt(name+"StartSpeedSeed")))
  ),
  acceleration(
      (rand()%2?1:-1)*Gamedata::getInstance()->getXmlInt(name+"AccelX"),
      (rand()%2?1:-1)*Gamedata::getInstance()->getXmlInt(name+"AccelY")
  ),
  maxSpeeds(
      Gamedata::getInstance()->getXmlInt(name+"SpeedXMax"),
      Gamedata::getInstance()->getXmlInt(name+"SpeedYMax")
  ),
  frame(fm)
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  acceleration(s.acceleration),
  maxSpeeds(s.maxSpeeds),
  frame(s.frame)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  frame = rhs.frame;
  acceleration = rhs.acceleration;
  maxSpeeds = rhs.maxSpeeds;
  return *this;
}

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
  Sprite::update();
}