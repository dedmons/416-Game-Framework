#include <cmath>
#include <algorithm>
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "manager.h"
#include "frameFactory.h"

Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i=0; i < sprites.size(); i++){
    delete sprites[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  jgdata( JSONGamedata::getInstance() ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  worlds(),
  viewport( Viewport::getInstance() ),
  sprites(),
  player(jgdata.getStr("player.name")),
  currentSprite(0),
  TICK_INTERVAL(jgdata.getInt("fpsController.tickInterval")),
  nextTime(clock.getTicks()+TICK_INTERVAL)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);

  JSONValue* backs = jgdata.getValue("backgrounds");
  unsigned int numbacks = backs->CountChildren();
  std::cout << "Loading " << numbacks << " backgrounds" << std::endl;
  for(unsigned i = 0; i < numbacks; i++){
    std::string name = backs->Child(i)->Child("name")->AsString();
    int fact = 1;
    if (backs->Child(i)->HasChild("fact"))
      fact = backs->Child(i)->Child("fact")->AsNumber();

    worlds.push_back( World(FrameFactory::getInstance().getFrame(name), fact) );
  }

  unsigned int n = jgdata.getInt("triForce.num");
  float smin = jgdata.getFloat("triForce.scale.min");
  float smax = jgdata.getFloat("triForce.scale.max");
  sprites.reserve(n+2);
  for(unsigned i = 0; i < n; i++){
    sprites.push_back(new AcceleratingSprite("triForce",smin,smax));
  }

  sort(sprites.begin(), sprites.end());

  viewport.setObjectToTrack(player.getSprite());
}

void Manager::draw() const {
  for(unsigned i = 0; i < worlds.size(); i++){
    worlds[i].draw();
  }
  for(unsigned i = 0; i < sprites.size(); i++){
    sprites[i]->draw();
  }
  player.draw();
  viewport.draw();

}

void Manager::update(){
    Uint32 ticks = clock.getElapsedTicks();
    for(unsigned i=0; i< sprites.size(); i++){
      sprites[i]->update(ticks);
    }
    player.update(ticks);
    viewport.update();
    for(unsigned i = 0; i < worlds.size(); i++){
      worlds[i].update();
    }
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
      viewport.drawHUD();

    std::stringstream sstm;
    sstm << "T_INT: " << TICK_INTERVAL+userTickInterval;
    IOManager::getInstance().printMessageAt(sstm.str(),viewport.W()-80,viewport.H()-20);

    SDL_Flip(screen);

    clock++;

    update();

    SDL_Delay(timeLeft());

    nextTime += TICK_INTERVAL+userTickInterval;

    SDL_PollEvent(&event);
    Uint8 *keyState = SDL_GetKeyState(NULL);
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
        case SDLK_o      : {
          if (!keyCatch)
          {
            keyCatch = true;
            viewport.setObjectToTrack(player.getSprite());
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
          break;
        }
        case SDLK_f      : {
          if(!keyCatch) {
              keyCatch = true;
              if (TICK_INTERVAL + userTickInterval > 0)
                  userTickInterval--;
          }
          break;
        }
        case SDLK_r      : {
          if(!keyCatch) {
              keyCatch = true;
              userTickInterval = 0;
          }
          break;
        }
        case SDLK_F1     : {
          if(!keyCatch) {
              keyCatch = true;
              showHelp = !showHelp;
          }
          break;
        }
        default          : break;
      }
      // bool playerKeyDown = false;
      if (keyState[SDLK_LEFT]){
        player.left();
        // playerKeyDown = true;
      }
      if (keyState[SDLK_RIGHT]){
        player.right();
        // playerKeyDown = true;
      }
      if (keyState[SDLK_UP]){
        player.up();
        // playerKeyDown = true;
      }
      if (keyState[SDLK_DOWN]){
        player.down();
        // playerKeyDown = true;
      }
    }
  }
}
