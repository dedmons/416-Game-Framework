#include <cmath>
#include <algorithm>
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "manager.h"
#include "frameFactory.h"
#include "sound.h"
#include "planets.h"
#include "collisionStrategy.h"

Manager::~Manager() {
  for (unsigned i=0; i < sprites.size(); i++){
    delete sprites[i];
  }

  std::list<Drawable*>::iterator ptr = explosions.begin();
  while( ptr != explosions.end() )
  {
    delete *ptr;
    ptr = explosions.erase(ptr);
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
  explosions(),
  sprites(),
  player(jgdata.getStr("player.name")),
  currentSprite(0),
  TICK_INTERVAL(jgdata.getInt("fpsController.tickInterval")),
  nextTime(clock.getTicks()+TICK_INTERVAL)
{
  clock.pause();
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
    sprites.push_back(new Sprite("triForce",smin,smax));
  }

  explosions.push_back(new TwowayMultiframeSprite("Etank"));

  sort(sprites.begin(), sprites.end());

  viewport.setObjectToTrack(player.getSprite());
}

void Manager::draw() const {

  for(unsigned i = 0; i < worlds.size(); i++){
    worlds[i].draw();
  }
  Planets::getInstance().draw();
  for(unsigned i = 0; i < sprites.size(); i++){
    sprites[i]->draw();
  }

  std::list<Drawable*>::const_iterator ptr = explosions.begin();
  while( ptr != explosions.end() )
  {
    (*ptr)->draw();
    ++ptr;
  }

  player.draw();
  viewport.draw();
  viewport.drawPlayerSpeed(player.getProjVel());
}

void Manager::update(){
  Uint32 ticks = clock.getElapsedTicks();
  for(unsigned i=0; i< sprites.size(); i++){
    sprites[i]->update(ticks);
  }
  std::list<Drawable*>::iterator ptr = explosions.begin();
  while( ptr != explosions.end() )
  {
    (*ptr)->update(ticks);

    ExplodingSprite *es = dynamic_cast<ExplodingSprite*>(*ptr);

    if(es && es->chunkCount() == 0){
      viewport.setObjectToTrack(player.getSprite());
      delete *ptr;
      ptr = explosions.erase(ptr);
    } else
      ++ptr;
  }
  if(!player.update(ticks))
    viewport.setObjectToTrack(player.getSprite());
  viewport.update();
  for(unsigned i = 0; i < worlds.size(); i++){
    worlds[i].update();
  }
}

int Manager::timeLeft(){
  int now = clock.getTicks();
  return std::max(0, nextTime - now);
}

void Manager::explodeSprite(const string& name) {
  std::list<Drawable*>::iterator ptr = explosions.begin();
  while ( ptr != explosions.end() ) {
    Drawable* sprite = *ptr;
    if ( sprite ){
      std::cout << "Checking: " << sprite->getName() << std::endl;
      if (sprite->getName() == name) {
        const Frame* frame = sprite->getFrame();
        Sprite newSprite(sprite->getPosition(), sprite->getVelocity(),
         name, frame);
        delete sprite;
        ptr = explosions.erase(ptr);
        explosions.push_back( new ExplodingSprite(newSprite) );
      }
    }
    else ++ptr;
  }
}

void Manager::checkCollisions(){
  player.checkCollisions();
}

void Manager::play() {
  clock.unpause();
  SDL_Event event;
  SDLSound sound;

  bool done = false;
  bool keyCatch = false;
  bool shiftKeyDown = false;
  bool showHelp = false;
  bool tankExploded = false;
  bool freeCam = false;

  int userTickInterval = 0;
  while ( ! done ) {
    draw();
    if (showHelp)
      viewport.drawHUD();

    SDL_Flip(screen);

    clock++;

    update();
    checkCollisions();
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
        case SDLK_SPACE : {
          if(!keyCatch){
            keyCatch = true;
            if(player.shoot()){
              sound[0];
              viewport.setObjectToTrack(player.getProj());
            }
          }
          break;
        }

        case SDLK_p      : {
          if (!keyCatch) {
            keyCatch = true;
            if ( clock.isPaused() ) clock.unpause();
            else clock.pause();
          }
          break;
        }
        case SDLK_e : {
          if(!keyCatch) {
            keyCatch = true;
            if(!tankExploded){
              tankExploded = true;
              explodeSprite("Etank");
              viewport.setObjectToTrack(explosions.back());
            }
          }
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
            if(sprites.size() == 0)
              break;
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
        case SDLK_f      : {
          if(!keyCatch) {
            keyCatch = true;
            freeCam = !freeCam;
            viewport.setFreeMode(freeCam);
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
      if (keyState[SDLK_LEFT]){
        if(freeCam) viewport.moveLeft();
        else player.changeMovement(Player::LEFT);
      }
      if (keyState[SDLK_RIGHT]){
        if(freeCam) viewport.moveRight();
        else player.changeMovement(Player::RIGHT);
      }
      if (keyState[SDLK_UP]){
        if(freeCam) viewport.moveUp();
        else player.changeMovement(Player::UP);
      }
      if (keyState[SDLK_DOWN]){
        if(freeCam) viewport.moveDown();
        else player.changeMovement(Player::DOWN);
      }
    }
  }
  clock.pause();
}
