#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include "jsongamedata.h"
#include "twowayMultisprite.h"
#include "collisionStrategy.h"

class Player {
public:
  Player(const std::string& name);
  virtual ~Player();

  virtual void update(Uint32 ticks);
    virtual void draw() const { drawable->draw(); strategy->draw(); }
  const Drawable* getSprite() const { return drawable; }

    enum MoveType { STOPUD, STOPLR, RIGHT, LEFT, UP, DOWN };
    void changeMovement(int);
    
    //bool collidedWith(const Drawable* d) const {
    //    return strategy->execute(*drawable, *d);
    //}
    
    void setCollisionStrategy(){
        strategy = strategies[0];
    }

private:
  const JSONGamedata& jgdata;
  bool keyPressedUD;
  bool keyPressedLR;
  int worldWidth;
  int worldHeight;
  Vector2f maxVel;
  float width;
  float height;
  Drawable * drawable;
    CollisionStrategy *strategy;
    std::vector<CollisionStrategy*> strategies;

  Player();
  Player(const Player&);
  Player& operator=(const Player&);
};
#endif
