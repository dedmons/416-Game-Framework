#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "clock.h"
#include "jsongamedata.h"

using std::cout; using std::endl;

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock;
  return clock;
}

Clock::Clock() :
  started(false),
  paused(false),
  frames(0),
  timeAtStart(0), timeAtPause(0),
  currTicks(0), prevTicks(0), ticks(0),
  fpsLogLength( JSONGamedata::getInstance().getInt("fpsController.fpsLogLength")),
  fpsLog(fpsLogLength)
  {
  start();
}

Clock::Clock(const Clock& c) :
  started(c.started),
  paused(c.paused), frames(c.frames),
  timeAtStart(c.timeAtStart), timeAtPause(c.timeAtPause),
  currTicks(c.currTicks), prevTicks(c.prevTicks), ticks(c.ticks),
  fpsLogLength(c.fpsLogLength), fpsLog(c.fpsLog)
  {
  start();
}

void Clock::debug( ) {
  cout << "The clock is:" << endl;
  cout << "\tstarted:" << started << endl;
  cout << "\tpaused:" << paused << endl;
  cout << "\tframes:" << frames << endl;
  cout << "\ttimeAtStart:" << timeAtStart << endl;
  cout << "\ttimeAtPause:" << timeAtPause << endl;
  cout << "\tcurrTicks:" << currTicks << endl;
  cout << "\tprevTicks:" << prevTicks << endl;
  cout << "\tticks:" << ticks << endl;
  cout << endl;
}

unsigned Clock::getTicks() const {
  if (paused) return timeAtPause;
  else return SDL_GetTicks() - timeAtStart;
}

unsigned Clock::getElapsedTicks() {
  if (paused) return 0;
  currTicks = getTicks();
  ticks = currTicks-prevTicks;
  prevTicks = currTicks;

  fpsLog.push_front(1/(ticks/1000.0));
  fpsLog.pop_back();

  return ticks;
}

float Clock::getFps(){
  int sum = 0;

  for(unsigned i=0; i < fpsLogLength; i++){
    sum += fpsLog[i];
  }

  return sum/fpsLogLength;
}

Clock& Clock::operator++() {
  if ( !paused ) ++frames;
  return *this;
}
Clock  Clock::operator++(int) {
  if ( !paused ) frames++;
  return *this;
}

void Clock::start() {
  started = true;
  paused = false;
  frames = 0;
  timeAtPause = timeAtStart = SDL_GetTicks();
}
void Clock::pause() {
  if( started && !paused ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    paused = true;
  }
}
void Clock::unpause() {
  if( started && paused ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    paused = false;
  }
}

