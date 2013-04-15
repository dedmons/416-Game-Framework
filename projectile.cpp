#include "projectile.h"
#include "sprite.h"
#include "explodingsprite.h"
#include "viewport.h"

Projectile::Projectile(const std::string& name, const Vector2f& vel, const Vector2f& pos) : 
	jgdata( JSONGamedata::getInstance() ),
  worldWidth( jgdata.getInt("world.width") ), 
  worldHeight( jgdata.getInt("world.height") ), 
  width( jgdata.getInt(name+".size.width") ), 
  height( jgdata.getInt(name+".size.height") ),
  drawable(new MultiframeSprite(name))
{
	drawable->setPosition(pos);
	drawable->setVelocity(vel);
}

Projectile::~Projectile(){
	delete drawable;
}

bool Projectile::update(Uint32 ticks){
	drawable->update(ticks);
	ExplodingSprite *es = dynamic_cast<ExplodingSprite*>(drawable);
	if(es && es->chunkCount() == 0){
		return false;
	}
	return true;
}

void Projectile::explode(){
	Drawable* tmp = drawable;
	const Frame* frame = tmp->getFrame();
  Sprite newSprite(tmp->getPosition(), tmp->getVelocity(), 
         tmp->getName(), frame);
  drawable = new ExplodingSprite(newSprite,jgdata.getInt(tmp->getName()+".chunk.size"));
  delete tmp;
  Viewport::getInstance().setObjectToTrack(drawable);
}
