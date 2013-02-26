#include <SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <deque>

class Manager;
class Clock {
public:
  static Clock& getInstance();
  unsigned getTicks() const;
  int getFps();

private:
  friend class Manager;
  unsigned getElapsedTicks();
  Clock& operator++();
  Clock  operator++(int);
  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned getFrames() const  { return frames;  }
  unsigned getSeconds() const { return getTicks()/1000;  }

  void start();
  void pause();
  void unpause();
  void debug();
  //void save(std::fstream& xmlfile) const;
  //void restore(const std::string& filename);

  bool started;
  bool paused;
  unsigned frames;
  unsigned timeAtStart;
  unsigned timeAtPause;
  unsigned currTicks;
  unsigned prevTicks;
  unsigned ticks;

  unsigned fpsLogLength;
  std::deque<int> fpsLog;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
