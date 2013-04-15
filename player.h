#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include "jsongamedata.h"
#include "twowayMultisprite.h"
#include "multisprite.h"
#include "projectile.h"

class Player {
public:
  Player(const std::string& name);
  virtual ~Player();

  virtual bool update(Uint32 ticks);
  virtual void draw() const { drawable->draw(); if(proj) proj->draw(); }
  const Drawable* getSprite() const { return drawable; }
  const Drawable* getProj() const { return proj->getSprite(); }

  enum MoveType { STOPUD, STOPLR, RIGHT, LEFT, UP, DOWN };
  void changeMovement(int);
  void shoot();
  void explodeShot();
  bool checkCollisions();

private:
  const JSONGamedata& jgdata;
  bool keyPressedUD;
  bool keyPressedLR;
  int worldWidth;
  int worldHeight;
  Vector2f maxVel;
  float width;
  float height;
  MultiframeSprite * drawable;
  
  Projectile * proj;

  Player();
  Player(const Player&);
  Player& operator=(const Player&);
};
#endif
