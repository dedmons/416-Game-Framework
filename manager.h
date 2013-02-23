#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "drawable.h"
#include "sprite.h"
#include "acceleratingSprite.h"
#include "world.h"
#include "viewport.h"

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
  World world;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  int currentSprite;

  void draw() const;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
