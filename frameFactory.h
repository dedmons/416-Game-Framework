#include <string>
#include <vector>
#include <map>
#include "frame.h"
#include "gamedata.h"

class FrameFactory {
public:
  static FrameFactory& getInstance();
  ~FrameFactory();
  Frame* getFrame(const std::string&);
  Frame* getFrame(const std::string&, const Uint16,
    const Uint16, const Uint16, const Uint16, const Uint16);
  std::vector<Frame*> getMultiFrames(const std::string&);

private:
  Gamedata &gdata;
  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, Frame*> frames;

  FrameFactory() :
    gdata( Gamedata::getInstance() ), surfaces(), frames()
  {}
  FrameFactory(const FrameFactory&);
  FrameFactory& operator=(const FrameFactory&);
};
