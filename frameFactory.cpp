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
    //std::cout << "Deleting Frame " << itFrame->first << std::endl;
    delete itFrame->second;
    ++itFrame;
  }
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
  return getFrame(name,
            0,
            gdata.getXmlInt(name+"Width"),
            gdata.getXmlInt(name+"Height"),
            gdata.getXmlInt(name+"SrcX"),
            gdata.getXmlInt(name+"SrcY"));
}

Frame* FrameFactory::getFrame(const std::string& name, const int num,
    const Uint16 width, const Uint16 height, const Uint16 srcX, const Uint16 srcY) {

  std::stringstream sstm;
  sstm << name << num;
  std::string fmName = sstm.str();

  //std::cout << "Getting frame: " << fmName << " => ";

  std::map<std::string, Frame*>::const_iterator pos = frames.find(fmName);
  if ( pos == frames.end() ) {
    //std::cout << "Making its frame" << std::endl;
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
    //std::cout << "Returning previously made frame" << std::endl;
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getMultiFrames(const std::string& name){
  unsigned numberofframes = gdata.getXmlInt(name+"Frames");
  std::vector<Frame*> retvector;
  retvector.reserve(numberofframes);

  Uint16 pwidth = gdata.getXmlInt(name+"Width");
  Uint16 pheight = gdata.getXmlInt(name+"Height");
  Uint16 srcx = gdata.getXmlInt(name+"SrcX");
  Uint16 srcy = gdata.getXmlInt(name+"SrcY");

  for (unsigned i = 0; i < numberofframes; ++i) {
    unsigned framex = i * pwidth + srcx;
    retvector.push_back(
      getFrame(name, i+1, pwidth, pheight, framex, srcy) );
  }

  return retvector;
}

std::vector<Frame*> FrameFactory::getLeftMultiFrames(const std::string& name){
  unsigned numberofframes = gdata.getXmlInt(name+"Frames");
  std::vector<Frame*> retvector;
  retvector.reserve(numberofframes);

  Uint16 pwidth = gdata.getXmlInt(name+"Width");
  Uint16 pheight = gdata.getXmlInt(name+"Height");
  Uint16 srcx = gdata.getXmlInt(name+"SrcX");
  Uint16 srcy = gdata.getXmlInt(name+"SrcY");

  for (unsigned i = 0; i < numberofframes; ++i) {
    unsigned framex = i * pwidth + srcx;
    unsigned framey = srcy + pheight;
    retvector.push_back(
      getFrame(name, -(i+1), pwidth, pheight, framex, framey) );
  }

  return retvector;
}
