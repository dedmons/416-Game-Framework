#include <string>
#include "jsongamedata.h"
#include "twowayMultisprite.h"

class Player {
public:
  Player(const std::string& name);
  virtual ~Player();

  virtual void update(Uint32 ticks);
  virtual void draw() const { drawable->draw(); }
  const Drawable* getSprite() const { return drawable; }

  void stop();
  void right();
  void left();
  void up();
  void down();

private:
  const JSONGamedata& jgdata;
  bool keyPressed;
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
