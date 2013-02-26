#include <iostream>
#include <cmath>
#include "twowayMultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void TwowayMultiframeSprite::advanceFrame(Uint32 ticks) {
  float ms = 1000.0/frameInterval;
  dt += ticks;
  int df = dt / ms;
  dt -= df * ms;
  currentFrame = (currentFrame + df) % numberOfFrames;
}

TwowayMultiframeSprite::TwowayMultiframeSprite( const std::string& name,
                  std::vector<Frame*> & fmsLeft,
                  std::vector<Frame*> & fmsRight) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"),
                    Gamedata::getInstance().getXmlInt(name+"Y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance().getXmlInt(name+"SpeedY"))
           ),
  framesLeft(fmsLeft),
  framesRight(fmsRight),
  frames(fmsRight),
  frameWidth(framesLeft[0]->getWidth()),
  frameHeight(framesLeft[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"FrameInterval") )
{ }

TwowayMultiframeSprite::TwowayMultiframeSprite( const std::string& name):
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"),
                    Gamedata::getInstance().getXmlInt(name+"Y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance().getXmlInt(name+"SpeedY"))
           ),
  framesLeft(FrameFactory::getInstance().getLeftMultiFrames(name)),
  framesRight(FrameFactory::getInstance().getMultiFrames(name)),
  frames(framesRight),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"Frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"FrameInterval") )
{ }

TwowayMultiframeSprite::
TwowayMultiframeSprite(const TwowayMultiframeSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  framesLeft(s.framesLeft),
  framesRight(s.framesRight),
  frames(s.framesRight),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  dt(s.dt),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval )
  { }

void TwowayMultiframeSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void TwowayMultiframeSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }

  if ( velocityX() < 0 ) frames = framesLeft;
  if ( velocityX() > 0 ) frames = framesRight;
}
