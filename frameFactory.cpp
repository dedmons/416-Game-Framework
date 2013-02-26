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

Frame* FrameFactory::getFrame(const std::string& name, const Uint16 num,
    const Uint16 width, const Uint16 height, const Uint16 srcX, const Uint16 srcY) {

  std::stringstream sstm;
  sstm << name << num;
  std::string fmName = sstm.str();

  std::map<std::string, Frame*>::const_iterator pos = frames.find(fmName);
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"File"),
          gdata.getXmlBool(name+"Transparency"));
    surfaces[fmName] = surface;
    Frame * const frame =new Frame(surface,
                width,
                height,
                srcX,
                srcY);
    frames[fmName] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getMultiFrames(const std::string& name){
  unsigned numberOfFrames = gdata.getXmlInt(name+"Frames");
  std::vector<Frame*> retVector;
  retVector.reserve(numberOfFrames);

  Uint16 pwidth = gdata.getXmlInt(name+"Width");
  Uint16 pheight = gdata.getXmlInt(name+"Height");
  Uint16 srcX = gdata.getXmlInt(name+"SrcX");
  Uint16 srcY = gdata.getXmlInt(name+"SrcY");

  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * pwidth + srcX;
    retVector.push_back(
      getFrame(name, i, pwidth, pheight, frameX, srcY) );
  }

  return retVector;
}

std::vector<Frame*> FrameFactory::getLeftMultiFrames(const std::string& name)
