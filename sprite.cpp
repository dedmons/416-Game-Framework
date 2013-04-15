#include <iostream>
#include <cmath>
#include <limits>
#include "sprite.h"
#include "jsongamedata.h"
#include "frameFactory.h"
#include "planets.h"
#include "collisionStrategy.h"


Sprite::Sprite(const Vector2f& pos, const Vector2f& vel, const std::string& name, const Frame* fm) :
  Drawable(name, pos, vel),
  scale(1),
  frame(fm),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  acceleration(Vector2f(0,0)),
  maxSpeeds(Vector2f(-99,-99)),
  mass(0)
{}

Sprite::Sprite(const std::string& name, const Vector2f& pos, const Vector2f& vel) :
  Drawable(name, pos, vel),
  scale(1),
  frame( FrameFactory::getInstance().getFrame(name, scale) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  acceleration(Vector2f(0,0)),
  maxSpeeds(Vector2f(-99,-99)),
  mass(0)
{}

Sprite::Sprite( const string& name, const float sMin, const float sMax) :
  Drawable(name,
           Vector2f(JSONGamedata::getInstance().getInt(name+".loc.start.x"),
                    JSONGamedata::getInstance().getInt(name+".loc.start.y")),
           Vector2f(
             ((rand()%2)?1:-1)*Random::getInstance().getRand(
                JSONGamedata::getInstance().getInt(name+".speed.min.x"),
                JSONGamedata::getInstance().getInt(name+".speed.start.x")),
             ((rand()%2)?1:-1)*Random::getInstance().getRand(
                  JSONGamedata::getInstance().getInt(name+".speed.min.y"),
                  JSONGamedata::getInstance().getInt(name+".speed.start.y")))
  ),
  scale( Random::getInstance().getRand(sMin,sMax) ),
  frame( FrameFactory::getInstance().getFrame(name, scale)) ,
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  acceleration(Vector2f(0,0)),
  maxSpeeds(Vector2f(-99,-99)),
  mass(0)
{ 
  if(JSONGamedata::getInstance().hasValue(name+".accel")){
    acceleration = Vector2f(
      ((rand()%2)?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.x"),
      ((rand()%2)?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.y")
    );
  }
  if(JSONGamedata::getInstance().hasValue(name+".speed.max")){
    maxSpeeds = Vector2f(
      JSONGamedata::getInstance().getInt(name+".speed.max.x"),
      JSONGamedata::getInstance().getInt(name+".speed.max.y")
    );
  }
  if(JSONGamedata::getInstance().hasValue(name+".mass")){
    mass = JSONGamedata::getInstance().getFloat(name+".mass");
  }
}

Sprite::Sprite( const string& name, const float s) :
  Drawable(name,
           Vector2f(JSONGamedata::getInstance().getInt(name+".loc.start.x"),
                    JSONGamedata::getInstance().getInt(name+".loc.start.y")),
           Vector2f(
             ((rand()%2)?1:-1)*Random::getInstance().getRand(
                JSONGamedata::getInstance().getInt(name+".speed.min.x"),
                JSONGamedata::getInstance().getInt(name+".speed.start.x")),
             ((rand()%2)?1:-1)*Random::getInstance().getRand(
                  JSONGamedata::getInstance().getInt(name+".speed.min.y"),
                  JSONGamedata::getInstance().getInt(name+".speed.start.y")))
  ),
  scale( s ),
  frame( FrameFactory::getInstance().getFrame(name, scale)) ,
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  acceleration(Vector2f(0,0)),
  maxSpeeds(Vector2f(-99,-99)),
  mass(0)
{ 
  if(JSONGamedata::getInstance().hasValue(name+".accel")){
    acceleration = Vector2f(
      ((rand()%2)?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.x"),
      ((rand()%2)?1:-1)*JSONGamedata::getInstance().getInt(name+".accel.y")
    );
  }
  if(JSONGamedata::getInstance().hasValue(name+".speed.max")){
    maxSpeeds = Vector2f(
      JSONGamedata::getInstance().getInt(name+".speed.max.x"),
      JSONGamedata::getInstance().getInt(name+".speed.max.y")
    );
  }
  if(JSONGamedata::getInstance().hasValue(name+".mass")){
    mass = JSONGamedata::getInstance().getFloat(name+".mass");
  }
}

Sprite::Sprite(const Sprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  scale(s.scale),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  acceleration(s.acceleration),
  maxSpeeds(s.maxSpeeds),
  mass(s.mass)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = JSONGamedata::getInstance().getInt("world.width");
  worldHeight = JSONGamedata::getInstance().getInt("world.height");
  acceleration = rhs.acceleration;
  maxSpeeds = rhs.maxSpeeds;
  return *this;
}

void Sprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
}

unsigned Sprite::getPixel(Uint32 i, Uint32 j) const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frame->getSurface()->pixels);
  return pixels[ ( y * frame->getWidth() ) + x ];
}

int Sprite::getDistance(const Sprite *obj) const {
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) {

  const static float speedLoss = JSONGamedata::getInstance().getFloat("sprite.speedLoss");

  updateVelocity(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001 * scale;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() * speedLoss) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() * speedLoss) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() * speedLoss) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() * speedLoss) );
  }

  if(Planets::getInstance().checkForCollision(this)){
    velocityY( -velocityY() * speedLoss );
    velocityX( -velocityX() * speedLoss );
  }
}

void Sprite::updateVelocity(Uint32 ticks){

  Planets &planets = Planets::getInstance();

  acceleration = planets.accelerationFromPlanets(getPosition());

  // std::cout << "Sprite Accel: " << acceleration << std::endl;

  float inc = acceleration[0] * 0.001 * static_cast<float>(ticks);
  velocityX( velocityX() + inc );

  inc = acceleration[1] * 0.001 * static_cast<float>(ticks);
  velocityY( velocityY() + inc );

  if(maxSpeeds[0] != -99){
    if(velocityX() > maxSpeeds[0]) {
      velocityX( maxSpeeds[0] );
    } else if(velocityX() < -maxSpeeds[0]){
      velocityX( -maxSpeeds[0]);
    }
  }

  if(maxSpeeds[1] != -99){
    if(velocityY() > maxSpeeds[1]) {
      velocityY( maxSpeeds[1] );
    } else if(velocityY() < -maxSpeeds[1]){
      velocityY( -maxSpeeds[1]);
    }
  }
}
