#ifndef PROJECTILE__H
#define PROJECTILE__H

#include "multisprite.h"
#include "jsongamedata.h"
#include "vector2f.h"

class Projectile {
public:
	Projectile(const std::string& name, const Vector2f& vel, const Vector2f& pos);
  virtual ~Projectile();

  virtual bool update(Uint32 ticks);
  virtual void draw() const { drawable->draw(); }
  const Drawable* getSprite() const { return drawable; }

  void explode();

private:
 	const JSONGamedata& jgdata;
  int worldWidth;
  int worldHeight;
  float width;
  float height;
  Drawable * drawable;

  Projectile();
  Projectile(const Projectile&);
  Projectile& operator=(const Projectile&);
};
#endif
