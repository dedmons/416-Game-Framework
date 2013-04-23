#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include "menuManager.h"
#include "levelManager.h"

MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  clock( Clock::getInstance() ),
  backColor(),
  menu(),
  manager()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  backColor.r = 0;
  backColor.g = 0;
  backColor.b = 0;
  atexit(SDL_Quit);
  LevelManager::getInstance();
}

void MenuManager::drawBackground() const {
  SDL_FillRect( screen, NULL,
      SDL_MapRGB(screen->format, backColor.r, backColor.g, backColor.b) );
  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( screen, NULL, screen, &dest );
}

void MenuManager::getFile() {
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  bool nameDone = false;
  const string msg("File path relative to executable: ");
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(&event) )
      switch (event.type) {
        case SDL_KEYDOWN: {
                            if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
                              done = true;
                            }
                            if (keystate[SDLK_RETURN]) {
                              nameDone = true;
                            }
                            io.buildString(event);
                          }
      }
    drawBackground();
    io.printStringAfterMessage(msg, 20, 120);
    if ( nameDone ) {
      std::string file = io.getString();
      std::stringstream strm;
      strm << "Okay -- loading:  " << file;
      io.printMessageAt(strm.str(), 20, 160);

      int num = LevelManager::getInstance().loadFile(file);

      strm.clear();
      strm.str(std::string());
      strm << "Loaded " << num << " levels from file";
      io.printMessageAt(strm.str(), 20, 200);

      SDL_Flip(screen);
      SDL_Delay(1000);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::printLevels(){
  IOManager& io = IOManager::getInstance().getInstance();
  int x = 20;
  int y = 50;
  int space = 30;
  int index = 0;
  drawBackground();
  std::stringstream strm;
  for (std::string s : LevelManager::getInstance().getLevelList()){
    strm << index++ << ": " << s;
    std::cout << strm.str() << std::endl;
    io.printMessageAt(strm.str(),x,y);
    y+=space;
    strm.clear();
    strm.str(std::string());
  }
  SDL_Flip(screen);
  SDL_Delay(2000);
}

int MenuManager::chooseLevel(){
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  bool levelDone = false;
  const string msg("Enter Level Number: ");
  int levelNum = -1;
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(&event) )
      switch (event.type) {
        case SDL_KEYDOWN: {
                            if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
                              done = true;
                            }
                            if (keystate[SDLK_RETURN]) {
                              levelDone = true;
                            }
                            else
                              io.buildString(event);
                          }
      }
    drawBackground();
    io.printStringAfterMessage(msg, 20, 120);
    if ( levelDone ) {
      if(io.getString() == "?"){
        printLevels();
        done = false;
        levelDone = false;
        io.clearString();
      } else {
        std::string level = io.getString();
        std::stringstream strm;
        strm << level;
        strm >> levelNum;
        strm.clear();
        strm.str(std::string());
        strm << "Okay -- loading:  " << levelNum;
        io.printMessageAt(strm.str(), 20, 160);

        LevelManager::getInstance().loadLevelNum(levelNum);

        SDL_Flip(screen);
        SDL_Delay(1000);
        done = true;
      }
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
  return levelNum;
}

void MenuManager::start() {
  JSONGamedata& jgdata = JSONGamedata::getInstance();
  bool keyCatch = false; // To get only 1 key per keydown
  SDL_Event event;
  bool done = false;
  // Here, we need to create an instance of the Manager,
  // the one that manages the game:
  // Manager manager;

  while ( not done ) {

    drawBackground();
    menu.draw();
    SDL_Flip(screen);

    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE :
        case SDLK_q :
          {
            done = true;
            break;
          }
        case SDLK_RETURN :
          {
            if ( !keyCatch ) {
              menu.lightOn();
              if ( menu.getIconClicked() == jgdata.getStr("menu.options.#0") ) {
                // Here is where we call the play() function in Manager:
                int level = chooseLevel();
                if(level != -1){
                  manager.play();
                }
              }
              if ( menu.getIconClicked() == jgdata.getStr("menu.options.#1") ) {
                getFile();
                // After we get the parameter, we must pass it to Manager:
                // manager.setNumberOfStars( numberOfStars );
              }
              if ( menu.getIconClicked() == jgdata.getStr("menu.options.#2") ) {
                // Here is where we explain how to play the game"
              }
              if ( menu.getIconClicked() == jgdata.getStr("menu.options.#3") ) {
                drawBackground();
                menu.draw();
                SDL_Flip(screen);
                SDL_Delay(250);
                done = true;
              }
            }
            break;
          }
        case SDLK_DOWN   :
          {
            if ( !keyCatch ) {
              menu.increment();
            }
            break;
          }
        case SDLK_UP   :
          {
            if ( !keyCatch ) {
              menu.decrement();
            }
            break;
          }
        default          : break;
      }
      keyCatch = true;
    }
    if(event.type == SDL_KEYUP) {
      keyCatch = false;
      menu.lightOff();
    }
  }
}
