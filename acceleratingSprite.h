#include "sprite.h"

class AcceleratingSprite : public Sprite {
public:
  AcceleratingSprite(const std::string&, const Frame*);
  AcceleratingSprite(const AcceleratingSprite&);
  virtual ~AcceleratingSprite() { };

  virtual void update(Uint32);
private:
  Vector2f acceleration;
  Vector2f maxSpeeds;

  void updateVelocity(Uint32);
};
