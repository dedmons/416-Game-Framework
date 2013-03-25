#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "jsongamedata.h"
#include "drawable.h"
#include "sprite.h"
#include "acceleratingSprite.h"
#include "world.h"
#include "viewport.h"
#include "player.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const Gamedata& gdata;
  const JSONGamedata& jgdata;
  const IOManager& io;
  Clock& clock;

  SDL_Surface *screen;

  std::vector<World> worlds;
  Viewport& viewport;

  std::vector<Sprite*> sprites;

  Player player;

  int currentSprite;
  const unsigned TICK_INTERVAL;
  int nextTime;

  void draw() const;
  void update();
  int timeLeft();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
