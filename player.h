#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include "jsongamedata.h"
#include "twowayMultisprite.h"
#include "multisprite.h"

class Player {
public:
  Player(const std::string& name);
  virtual ~Player();

  virtual void update(Uint32 ticks);
  virtual void draw() const { drawable->draw(); }
  const Drawable* getSprite() const { return drawable; }

  enum MoveType { STOPUD, STOPLR, RIGHT, LEFT, UP, DOWN };
  void changeMovement(int);

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

  Player();
  Player(const Player&);
  Player& operator=(const Player&);
};
#endif
