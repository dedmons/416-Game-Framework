#ifndef __MANAGER__H
#define __MANAGER__H

#include <SDL.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "ioManager.h"
#include "clock.h"
#include "jsongamedata.h"
#include "drawable.h"
#include "sprite.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "explodingSprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const JSONGamedata& jgdata;
  const IOManager& io;
  Clock& clock;

  SDL_Surface *screen;

  std::vector<World> worlds;
  Viewport& viewport;
  std::list<Drawable*> explosions;
  std::vector<Drawable*> sprites;
  Player player;

  int currentSprite;
  const unsigned TICK_INTERVAL;
  int nextTime;

  bool checkForCollisions() const;
  void draw() const;
  void update();
  int timeLeft();
  void explodeSprite(const string&);
  void checkCollisions();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
};

#endif
