#include <iostream>
#include "world.h"

void World::update() {
  viewX = static_cast<int>(view.X() / factor) % frameWidth;
  viewY = static_cast<int>(view.Y() / factor) % frameHeight;
}

void World::draw() const {
  unsigned y = 0;

  while (y*frameHeight <= worldHeight+frameHeight){
		unsigned x = 0;
  	while (x*frameWidth <= worldWidth+frameWidth){
  		frame1.draw(viewX*(x==0?1:0),viewY*(y==0?1:0),
  			(x*frameWidth)-(viewX*(x==0?0:1)),
  			(y*frameHeight)-(viewY*(y==0?0:1)));
  		x++;
  	}
  	y++;
	}
}

