#include "player.h"
#include "ioManager.h"

Player::~Player() {
  delete drawable;
}

Player::Player(const std::string& name) :
  jgdata( JSONGamedata::getInstance() ),
  keyPressed(false),
  worldWidth( jgdata.getInt("world.width") ), 
  worldHeight( jgdata.getInt("world.height") ), 
  maxVel( Vector2f(jgdata.getInt(name+".speed.max.x"), 
                               jgdata.getInt(name+".speed.max.y") )
  ), 
  width( jgdata.getInt(name+".size.width") ), 
  height( jgdata.getInt(name+".size.height") ),
  drawable(NULL)
{ 
   drawable = new TwowayMultiframeSprite(name);
}

void Player::update(Uint32 ticks) { 
  drawable->update(ticks); 
  if (!keyPressed ) stop();
  keyPressed = false;
}

void Player::stop() { 
  drawable->velocityX(0);  
  drawable->velocityY(0);  
}

void Player::right() { 
  keyPressed = true;
  if ( drawable->X() < worldWidth-width) {
    drawable->velocityX(maxVel[0]);
  }
} 
void Player::left()  { 
  keyPressed = true;
  if ( drawable->X() > 0) {
    drawable->velocityX(-maxVel[0]);
  }
} 
void Player::up()    { 
  keyPressed = true;
  if ( drawable->Y() < 0 ) {
    drawable->velocityY( abs( drawable->velocityY() ) );
  }
  else {
    drawable->velocityY(-maxVel[0]);
  }
} 
void Player::down()  { 
  keyPressed = true;
  if ( drawable->Y() < worldHeight-height) {
    drawable->velocityY(maxVel[0]);
  }
} 
