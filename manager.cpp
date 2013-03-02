#include <cmath>
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "manager.h"

Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i=0; i < sprites.size(); i++){
    delete sprites[i];
  }
  delete backFrame;
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
  currentSprite(0),
  TICK_INTERVAL(gdata.getXmlInt("tickInterval")),
  nextTime(clock.getTicks()+TICK_INTERVAL)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);

  unsigned int n = gdata.getXmlInt("triForceNum");
  sprites.reserve(n+2);
  for(unsigned i = 0; i < n; i++){
    sprites.push_back(new AcceleratingSprite("triForce"));
  }

  sprites.push_back( new MultiframeSprite("tank") );
  sprites.push_back( new TwowayMultiframeSprite("tank2"));

  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::draw() const {
  world.draw();
  for(unsigned i = 0; i < sprites.size(); i++){
    sprites[i]->draw();
  }
  viewport.draw();

}

void Manager::update(){
    Uint32 ticks = clock.getElapsedTicks();
    for(unsigned i=0; i< sprites.size(); i++){
      sprites[i]->update(ticks);
    }
    viewport.update();
    world.update();
}

int Manager::timeLeft(){
    int now = clock.getTicks();
    return std::max(0, nextTime - now);
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  bool shiftKeyDown = false;
  bool showHelp = false;

  int userTickInterval = 0;
  while ( ! done ) {
    draw();
    if (showHelp)
      viewport.drawHelp();
    std::stringstream sstm;
    sstm << "T_INT: " << TICK_INTERVAL+userTickInterval;
    IOManager::getInstance().printMessageAt(sstm.str(),10,25);

    SDL_Flip(screen);

    clock++;

    update();

    SDL_Delay(timeLeft());

    nextTime += TICK_INTERVAL+userTickInterval;

    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYUP) {
      keyCatch = false;
      switch (event.key.keysym.sym) {
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
          shiftKeyDown = false;
          break;

        default:
          break;
      }
    }
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
            if (shiftKeyDown)
              currentSprite = (currentSprite+sprites.size()-1)%sprites.size();
            else
              currentSprite = (currentSprite+1)%sprites.size();
            viewport.setObjectToTrack(sprites[currentSprite]);
          }
          break;
        }
        case SDLK_LSHIFT :
        case SDLK_RSHIFT : {
          if (!shiftKeyDown) {
            shiftKeyDown = true;
          }
          break;
        }
        case SDLK_s      : {
          if(!keyCatch) {
              keyCatch = true;
              userTickInterval = (userTickInterval+1);
          }
        }
        case SDLK_f      : {
          if(!keyCatch) {
              keyCatch = true;
              if (TICK_INTERVAL + userTickInterval > 0)
                  userTickInterval--;
          }
        }
        case SDLK_r      : {
          if(!keyCatch) {
              keyCatch = true;
              userTickInterval = 0;
          }
        }
        case SDLK_F1     : {
          if(!keyCatch) {
              keyCatch = true;
              showHelp = !showHelp;
          }
        }
        default          : break;
      }
    }
  }
}
