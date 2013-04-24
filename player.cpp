#include "player.h"
#include "planets.h"

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
  projVel(50),
  projExpl(false),
  drawable(NULL),
  proj(NULL)
{
   drawable = new MultiframeSprite(name);
   drawable->setManualFrames(true);
   drawable->setPosition(Vector2f(50,worldHeight-40));
}

bool Player::update(Uint32 ticks) {
  if(proj)
    if(!proj->update(ticks)){
      projExpl = false;
      delete proj;
      proj = NULL;
      return false;
    }
  return true;
}

void Player::explodeShot(){
  if(proj){
    projExpl = true;
    proj->explode();
  }
}

bool Player::checkCollisions(){
  if(proj && !projExpl){
    if(Planets::getInstance().checkForCollision(proj->getSprite())){
      explodeShot();
      return true;
    }
  }
  return false;
}

void Player::shoot() {
  if(proj != NULL)
    return;

  int fn = drawable->getFrameNumber();
  int dx = 0;
  int dy = 0;
  switch(fn){
    case  0: dx=40;dy=16; break;
    case  1: dx=39;dy=20; break;
    case  2: dx=38;dy=22; break;
    case  3: dx=36;dy=25; break;
    case  4: dx=35;dy=29; break;
    case  5: dx=32;dy=31; break;
    case  6: dx=30;dy=33; break;
    case  7: dx=26;dy=34; break;
    case  8: dx=23;dy=35; break;
    case  9: dx=20;dy=35; break;
    case 10: dx=17;dy=35; break;
    case 11: dx=14;dy=34; break;
    case 12: dx=10;dy=33; break;
    case 13: dx= 8;dy=31; break;
    case 14: dx= 5;dy=29; break;
    case 15: dx= 4;dy=25; break;
    case 16: dx= 2;dy=22; break;
    case 17: dx= 1;dy=20; break;
    case 18: dx= 0;dy=16; break;
  }

  Vector2f loc = Vector2f(drawable->X()+dx,drawable->Y()+(40-dy));
  Vector2f org = Vector2f(drawable->X()+20,drawable->Y()+25);

  Vector2f speed = loc-org;

  proj = new Projectile("proj",speed*projVel,loc);
}

void Player::changeMovement(int type)
{
    int fn = drawable->getFrameNumber();
    int mfn = drawable->getNumFrames();
    switch(type)
    {
        case Player::RIGHT :
            if ( fn > 0 ) {
                drawable->setFrameNumber(fn - 1);
            }
            break;
        case Player::LEFT :
            if ( fn < mfn-1) {
                drawable->setFrameNumber(fn + 1);
            }
            break;
        case Player::UP :
            if ( projVel < 100) {
                projVel += 5;
            }
            break;
        case Player::DOWN :
          if ( projVel > 10) {
                projVel -= 5;
            }
            break;
        default :
        break;
    }
}
