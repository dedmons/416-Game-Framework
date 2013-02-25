#include <cmath>
#include "manager.h"

Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i=0; i < sprites.size(); i++){
    delete sprites[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  gdata( Gamedata::getInstance() ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  backSurface( io.loadAndSet(gdata.getXmlStr("backFile"),
                gdata.getXmlBool("triForceTransparency"))
  ),
  backFrame(new Frame(backSurface,
                gdata.getXmlInt("backWidth"),
                gdata.getXmlInt("backHeight"),
                gdata.getXmlInt("backSrcX"),
                gdata.getXmlInt("backSrcY"))
  ),
  world( backFrame ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);

  unsigned int n = gdata.getXmlInt("triForceNum");
  sprites.reserve(n);
  for(unsigned i = 0; i < n; i++){
    sprites.push_back(new AcceleratingSprite("triForce"));
  }

  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::draw() const {
  world.draw();
  for(unsigned i = 0; i < sprites.size(); i++){
    sprites[i]->draw();
  }
  viewport.draw();
  SDL_Flip(screen);
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  while ( ! done ) {
    clock++;
    draw();

    Uint32 ticks = clock.getElapsedTicks();
    for(unsigned i=0; i< sprites.size(); i++){
      sprites[i]->update(ticks);
    }

    viewport.update();
    world.update();

    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYUP) { keyCatch = false; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE : done = true; break;
        case SDLK_q      : done = true; break;
        case SDLK_p      : {
          if (!keyCatch) {
            keyCatch = true;
            if ( clock.isPaused() ) clock.unpause();
            else clock.pause();
          }
          break;
        }
        case SDLK_t      : {
          if (!keyCatch) {
            keyCatch = true;
            currentSprite = (currentSprite+1)%sprites.size();
            viewport.setObjectToTrack(sprites[currentSprite]);
          }
        }
        default          : break;
      }
    }
  }
}
