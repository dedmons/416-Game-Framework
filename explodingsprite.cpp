#include <iostream>
#include <cmath>
#include <string>
#include "explodingsprite.h"
#include "random.h"

ExplodingSprite::ExplodingSprite(const Sprite& s) :
  Sprite(s),
  chunks(),
  freeList(),
  frames(),
  maxTicks(JSONGamedata::getInstance().getInt("chunks.maxTicks")),
  curTicks(0)
{
  makeChunks(JSONGamedata::getInstance().getInt("chunks.size"));
}

ExplodingSprite::ExplodingSprite(const Sprite& s, const int chunks) :
  Sprite(s),
  chunks(),
  freeList(),
  frames(),
  maxTicks(JSONGamedata::getInstance().getInt("chunks.maxTicks")),
  curTicks(0)
{
  makeChunks(chunks);
}

ExplodingSprite::~ExplodingSprite() {
  for ( unsigned int i = 0; i < frames.size(); ++i ) {
    delete frames[i]; // ExplodingSprite made them, so it deletes them
  }
  chunks.clear();     // clearing a vector is a waste of time, but ...
  freeList.clear();   // still ...
}

void ExplodingSprite::draw() const {
  // Override draw; use the draw in Chunk, which derives from Sprite.
  // So the draw we're using is actually in Sprite
  std::list<Chunk>::const_iterator ptr = chunks.begin();
  while (ptr != chunks.end()) {
    ptr->draw();
    ++ptr;
  }
}

void ExplodingSprite::update(Uint32 ticks) {
  Sprite::update(ticks);
  curTicks += ticks;
  std::list<Chunk>::iterator ptr = chunks.begin();
  while (ptr != chunks.end()) {
    ptr->update(ticks);
    if (curTicks > maxTicks) {  // Check to see if we should free a chunk
      if(rand()%20 == 0) {
        freeList.push_back(*ptr);
        ptr = chunks.erase(ptr);
      }
      else ++ptr;
    }
    else ++ptr;
  }
}

void ExplodingSprite::makeChunks(unsigned int n) {
  // Break the SDL_Surface into n*n squares; where each square
  // has width and height of frameWidth/n and frameHeight/n
  unsigned int chunk_width = frame->getWidth()/n;
  unsigned int chunk_height = frame->getHeight()/n;
  Sint16 source_x = frame->getSourceX();
  Sint16 source_y = frame->getSourceY();

  // Get the SDL_Surface so we can chunk it:
  SDL_Surface* spriteSurface(frame->getSurface());
  // i tracks the width, j tracks the height:
  for (unsigned int i = 0; i <= n; ++i) {
    for (unsigned int j = 0; j <= n; ++j) {
      // Give each chunk it's own speed/direction:

      Vector2f loc = Vector2f(i*chunk_width,j*chunk_height);
      Vector2f center = Vector2f(frame->getWidth()/2,frame->getHeight()/2);

      Vector2f dist = center - loc;

      float randx = Random::getInstance().getRand(1.25);

      Vector2f speed = dist*randx;
      speed += getVelocity();

      // speed *= 0; //Cool looking affect on tank.

      Frame* frame =
            new Frame(spriteSurface, chunk_width, chunk_height,
                  source_x+i*chunk_width,  // x coord of surface
                  source_y+j*chunk_height // y coord of surface
                );
      std::string name;
      //std::stringstream strm;
      //strm << getName();
      //strm << "Chunk"
      //name = strm.str();
      name = getName() + "Chunk";
      Chunk chunk(
                Vector2f(X()+i*chunk_width,Y()+j*chunk_height),
                speed,
                name,
                frame);

      chunks.push_back(chunk);
      frames.push_back(frame);
    }
  }
}

