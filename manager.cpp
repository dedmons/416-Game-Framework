#include <cmath>
#include "manager.h"

Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  SDL_FreeSurface(orbSurface);
  SDL_FreeSurface(backSurface);
  delete orbFrame;
  delete backFrame;
  delete Gamedata::getInstance();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  gdata( Gamedata::getInstance() ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  backSurface( io.loadAndSet(gdata->getXmlStr("backFile"), true) ),
  backFrame(new Frame(backSurface,
                gdata->getXmlInt("backWidth"),
                gdata->getXmlInt("backHeight"),
                gdata->getXmlInt("backSrcX"),
                gdata->getXmlInt("backSrcY"))
  ),
  background("background",backFrame),
  orbSurface( io.loadAndSet(gdata->getXmlStr("redorbFile"), true) ),
  orbFrame(new Frame(orbSurface,
                gdata->getXmlInt("redorbWidth"),
                gdata->getXmlInt("redorbHeight"),
                gdata->getXmlInt("redorbSrcX"),
                gdata->getXmlInt("redorbSrcY"))
  ),
  orbs()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);

  unsigned int n = gdata->getXmlInt("redorbNum");
  for(unsigned i = 0; i < n; i++){
    orbs.push_back(Sprite("redorb",orbFrame));
  }

}

void Manager::draw() const {
  //SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255) );
  //SDL_Rect dest = {0, 0, 0, 0};
  //SDL_BlitSurface( screen, NULL, screen, &dest );

  background.draw();
  for(unsigned i = 0; i < orbs.size(); i++){
    orbs[i].draw();
  }
  SDL_Flip(screen);
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  while ( ! done ) {
    draw();

    Uint32 ticks = clock.getElapsedTicks();
    for(unsigned i=0; i< orbs.size(); i++){
      orbs[i].update(ticks);
    }

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
        default          : break;
      }
    }
  }
}
