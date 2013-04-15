#include <iostream>
#include <cmath>
#include "multisprite.h"
#include "jsongamedata.h"
#include "frameFactory.h"

void MultiframeSprite::advanceFrame(Uint32 ticks) {
  float ms = 1000.0/frameInterval;
  dt += ticks;
  int df = dt / ms;
  dt -= df * ms;
  currentFrame = (currentFrame + df) % numberOfFrames;
}

MultiframeSprite::MultiframeSprite( const std::string& name,
                  const std::vector<Frame*> & fms) :
  Sprite(name,
           Vector2f(JSONGamedata::getInstance().getInt(name+".loc.start.x"),
                    JSONGamedata::getInstance().getInt(name+".loc.start.y")),
           Vector2f(JSONGamedata::getInstance().getInt(name+".speed.start.x"),
                    JSONGamedata::getInstance().getInt(name+".speed.start.y"))
           ),
  frames(fms),
  frameWidth(fms[0]->getWidth()),
  frameHeight(fms[0]->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  dt(0),
  currentFrame(0),
  numberOfFrames( JSONGamedata::getInstance().getInt(name+".frames.num") ),
  frameInterval( JSONGamedata::getInstance().getInt(name+".frames.interval") ),
  frameNumber(0),
  manualFrameCTL(false)
{ }

MultiframeSprite::MultiframeSprite( const std::string& name) :
  Sprite(name,
           Vector2f(JSONGamedata::getInstance().getInt(name+".loc.start.x"),
                    JSONGamedata::getInstance().getInt(name+".loc.start.y")),
           Vector2f(JSONGamedata::getInstance().getInt(name+".speed.start.x"),
                    JSONGamedata::getInstance().getInt(name+".speed.start.y"))
           ),
  frames( FrameFactory::getInstance().getMultiFrames(name) ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  dt(0),
  currentFrame(0),
  numberOfFrames( JSONGamedata::getInstance().getInt(name+".frames.num") ),
  frameInterval( JSONGamedata::getInstance().getInt(name+".frames.interval") ),
  frameNumber(0),
  manualFrameCTL(false)
{ }

MultiframeSprite::MultiframeSprite(const MultiframeSprite& s) :
  Sprite(s.getName(), s.getPosition(), s.getVelocity()),
  frames(s.frames),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(JSONGamedata::getInstance().getInt("world.width")),
  worldHeight(JSONGamedata::getInstance().getInt("world.height")),
  dt(s.dt),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  frameNumber(s.frameNumber),
  manualFrameCTL(s.manualFrameCTL)
  { }

void MultiframeSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if (manualFrameCTL)
    frames[frameNumber]->draw(x,y);
  else
    frames[currentFrame]->draw(x, y);
}

void MultiframeSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Sprite::update(ticks);

  // Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  // setPosition(getPosition() + incr);

  // if ( Y() < 0) {
  //   velocityY( abs( velocityY() ) );
  // }
  // if ( Y() > worldHeight-frameHeight) {
  //   velocityY( -abs( velocityY() ) );
  // }

  // if ( X() < 0) {
  //   velocityX( abs( velocityX() ) );
  // }
  // if ( X() > worldWidth-frameWidth) {
  //   velocityX( -abs( velocityX() ) );
  // }
}
