#include "sprite.h"

class AcceleratingSprite : public Sprite {
public:
  AcceleratingSprite(const std::string& n, const Frame*);
  AcceleratingSprite(const AcceleratingSprite& o);
  virtual ~AcceleratingSprite() { };

  virtual update(Uint32 ticks);
private:
  Vector2f acceleration;

  void updateVelocity(Uint32 ticks);
}
