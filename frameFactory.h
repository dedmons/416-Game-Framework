#include <string>
#include <vector>
#include <map>
#include "frame.h"
#include "jsongamedata.h"

class FrameFactory {
public:
  static FrameFactory& getInstance();
  ~FrameFactory();
  
  Frame* getFrame(const std::string&);
  Frame* getFrame(const std::string&, const float);
  Frame* getFrame(const std::string&, const int, const float,
    const int, const Uint16, const Uint16, const Uint16, const Uint16);
  
  std::vector<Frame*> getMultiFrames(const std::string&);
  std::vector<Frame*> getLeftMultiFrames(const std::string&);

private:
  JSONGamedata &jgdata;
  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, Frame*> frames;

  FrameFactory() :
    jgdata( JSONGamedata::getInstance() ), surfaces(), frames()
  {}
  FrameFactory(const FrameFactory&);
  FrameFactory& operator=(const FrameFactory&);
};
