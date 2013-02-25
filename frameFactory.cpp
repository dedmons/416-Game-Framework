#include "frameFactory.h"
#include "ioManager.h"
#include "vector2f.h"

FrameFactory::~FrameFactory() {
  std::cout << "The FrameFactory is down sizing!" << std::endl;
  std::map<std::string, SDL_Surface*>::iterator itSurf = surfaces.begin();
  while ( itSurf != surfaces.end() ) {
    SDL_FreeSurface( itSurf->second );
    ++itSurf;
  }
  std::map<std::string, Frame*>::iterator itFrame = frames.begin();
  while ( itFrame != frames.end() ) {
    delete itFrame->second;
    ++itFrame;
  }
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
  std::map<std::string, Frame*>::const_iterator pos = frames.find(name);
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"File"),
          gdata.getXmlBool(name+"Transparency"));
    surfaces[name] = surface;
    Frame * const frame =new Frame(surface,
                gdata.getXmlInt(name+"Width"),
                gdata.getXmlInt(name+"Height"),
                gdata.getXmlInt(name+"SrcX"),
                gdata.getXmlInt(name+"SrcY"));
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

