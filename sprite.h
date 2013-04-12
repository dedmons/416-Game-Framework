#ifndef SPRITE__H
#define SPRITE__H

#include <string>
#include <iostream>

#include "drawable.h"
#include "random.h"

class Sprite : public Drawable {
public:
  // Sprite(const std::string& n, const Frame*);
  Sprite(const std::string& n, const float scaleMin, const float scaleMax);
  Sprite(const std::string& n, const float scale = 1);
  Sprite(const Sprite& s);
  virtual ~Sprite() { }
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void setFrame(const Frame* f) { frame = f; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  unsigned getPixel(Uint32, Uint32) const;

  int getScale() const { return scale; }

  Vector2f getCenter() const {
    return Vector2f( X()+frame->getWidth()/2, Y()+frame->getHeight()/2 );
  }

  bool operator<(const Sprite& rhs){
    return scale < rhs.scale;
  }
protected:
  const Frame * frame;
private:
  float scale;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Sprite*) const;
};
#endif
