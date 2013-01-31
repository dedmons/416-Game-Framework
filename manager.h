#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "sprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const Gamedata* gdata;
  const IOManager& io;
  Clock& clock;

  SDL_Surface *screen;

  SDL_Surface * const backSurface;
  Frame * const backFrame;
  Sprite background;

  SDL_Surface * const orbSurface;
  Frame * const orbFrame;
  std::vector<Sprite> orbs;

  void draw() const;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
