#ifndef MULTISPRITE__H
#define MULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>

#include "sprite.h"

class MultiframeSprite : public Sprite {
public:
  MultiframeSprite(const std::string& n, const std::vector<Frame*>& fms);
  MultiframeSprite(const std::string& n);
  MultiframeSprite(const MultiframeSprite& s);
  virtual ~MultiframeSprite() { }
  virtual const Frame* getFrame() const { return frames[currentFrame]; }

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void setManualFrames(bool man = false) { manualFrameCTL = man; }
  void setFrameNumber(int num) { frameNumber = num; }
  int getNumFrames() { return numberOfFrames; }
  int getFrameNumber() { return frameNumber; }

private:
  const std::vector<Frame *> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  unsigned dt;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  int frameNumber;
  bool manualFrameCTL;

  void advanceFrame(Uint32 ticks);
};
#endif
