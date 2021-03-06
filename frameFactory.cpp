#include "frameFactory.h"
#include "ioManager.h"
#include "vector2f.h"
#include "random.h"

#include <SDL/SDL_rotozoom.h>

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
  return getFrame(name,
            0, 1, 1,
            jgdata.getInt(name+".size.width"),
            jgdata.getInt(name+".size.height"),
            jgdata.getInt(name+".src.x"),
            jgdata.getInt(name+".src.y"));
}

Frame* FrameFactory::getFrame(const std::string& name, const float scale) {
  return getFrame(name,
            0, scale, 1,
            jgdata.getInt(name+".size.width"),
            jgdata.getInt(name+".size.height"),
            jgdata.getInt(name+".src.x"),
            jgdata.getInt(name+".src.y"));
}

Frame* FrameFactory::getFrame(const std::string& name, const int num, const float scale,
    const int numFrames, const Uint16 width, const Uint16 height, const Uint16 srcX, const Uint16 srcY) {

  std::stringstream sstm;

  float realScale = roundf(scale*10.0)/10.0;

  sstm << name << num << "sc" << realScale;
  std::string fmName = sstm.str();

  bool debug = false;

  if(debug)
    std::cout << "Getting frame: " << fmName << " => ";

  std::map<std::string, Frame*>::const_iterator pos = frames.find(fmName);
  if ( pos == frames.end() ) {
    if(debug)
      std::cout << "Making its frame" << std::endl;
    SDL_Surface * surface;
    SDL_Surface * tmp =
      IOManager::getInstance().loadAndSet(
          jgdata.getStr(name+".file"),
          jgdata.getBool(name+".transparency"));

    float newScale = 1.0f;

    if(scale != 1){
      double oldW = tmp->w;
      double newW = numFrames*ceil((oldW*realScale-0.5)/numFrames);
      newScale = newW/oldW;

      surface = rotozoomSurface(tmp, 0, newScale, SMOOTHING_ON);
      SDL_FreeSurface(tmp);
    } else {
      surface = tmp;
    }

    surfaces[fmName] = surface;
    Frame * const frame =new Frame(surface,
                width*newScale,
                height*newScale,
                srcX*newScale,
                srcY*newScale);

    frames[fmName] = frame;
    return frame;
  }
  else {
    if(debug)
      std::cout << "Returning previously made frame" << std::endl;
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getMultiFrames(const std::string& name){
  unsigned numberofframes = jgdata.getInt(name+".frames.num");
  std::vector<Frame*> retvector;
  retvector.reserve(numberofframes);

  Uint16 pwidth = jgdata.getInt(name+".size.width");
  Uint16 pheight = jgdata.getInt(name+".size.height");
  Uint16 srcx = jgdata.getInt(name+".src.x");
  Uint16 srcy = jgdata.getInt(name+".src.y");
  Uint16 xoffset = jgdata.getInt(name+".src.offset.x");

  float scale = 1;
  if (jgdata.hasValue(name+".scale"))
    scale = jgdata.getFloat(name+".scale");

  for (unsigned i = 0; i < numberofframes; ++i) {
    unsigned framex = i * (pwidth + xoffset) + srcx;
    retvector.push_back(
      getFrame(name, i+1, scale, numberofframes, pwidth, pheight, framex, srcy) );
  }

  return retvector;
}

std::vector<Frame*> FrameFactory::getLeftMultiFrames(const std::string& name){
  unsigned numberofframes = jgdata.getInt(name+".frames.num");
  std::vector<Frame*> retvector;
  retvector.reserve(numberofframes);

  Uint16 pwidth = jgdata.getInt(name+".size.width");
  Uint16 pheight = jgdata.getInt(name+".size.height");
  Uint16 srcx = jgdata.getInt(name+".src.x");
  Uint16 srcy = jgdata.getInt(name+".src.y");
  Uint16 xoffset = jgdata.getInt(name+".src.offset.x");
  Uint16 yoffset = jgdata.getInt(name+".src.offset.y");

  float scale = 1;
  if (jgdata.hasValue(name+".scale"))
    scale = jgdata.getFloat(name+".scale");

  for (unsigned i = 0; i < numberofframes; ++i) {
    unsigned framex = i * (pwidth + xoffset) + srcx;
    unsigned framey = srcy + pheight + (yoffset);
    retvector.push_back(
      getFrame(name, -(i+1), scale, numberofframes, pwidth, pheight, framex, framey) );
  }

  return retvector;
}
