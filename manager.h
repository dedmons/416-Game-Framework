#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
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

  SDL_Surface * const triForceSurface;
  Frame * const triForceFrame;
  std::vector<Sprite*> sprites;
  int currentSprite;

  void draw() const;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
