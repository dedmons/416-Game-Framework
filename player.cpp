#include "player.h"

Player::~Player() {
  delete drawable;
}

Player::Player(const std::string& name) :
  jgdata( JSONGamedata::getInstance() ),
  keyPressedUD(false),
  keyPressedLR(false),
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
    if(keyPressedUD || keyPressedLR) drawable->update(ticks);
    if(!keyPressedUD) changeMovement(Player::STOPUD);
    if(!keyPressedLR) changeMovement(Player::STOPLR);
    keyPressedUD = keyPressedLR = false;
}

void Player::changeMovement(int type)
{
    switch(type)
    {
        case Player::RIGHT :
            keyPressedLR = true;
            if ( drawable->X() < worldWidth-width) {
                drawable->velocityX(maxVel[0]);
            }
            break;
        case Player::LEFT :
            keyPressedLR = true;
            if ( drawable->X() > 0) {
                drawable->velocityX(-maxVel[0]);
            }
            break;
        case Player::UP :
            keyPressedUD = true;
            if ( drawable->Y() < 0 ) {
                drawable->velocityY( abs( drawable->velocityY() ) );
            }
            else {
                drawable->velocityY(-maxVel[0]);
            }
            break;
        case Player::DOWN :
            keyPressedUD = true;
            if ( drawable->Y() < worldHeight-height) {
                drawable->velocityY(maxVel[0]);
            }
            break;
        case Player::STOPUD :
            keyPressedUD = false;
            drawable->velocityY(0);
        break;
        case Player::STOPLR :
            keyPressedLR = false;
            drawable->velocityX(0);
        break;
        default :
        break;
    }
}
