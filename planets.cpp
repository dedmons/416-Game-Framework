#include "planets.h"

Planets& Planets::getInstance(){
  static Planets instance;
  return instance;
}

Planets::Planets():planets(){
  //JSONValue* def = JSON::Parse(std::string("levels/defualt.json"));
  //if(loadPlanets(def->ChildAtPath("levels.#0"), def->Child("Planets")))
  //  std::cout << "Loaded default levels" << std::endl;
  //else
  //  std::cout << "Error loading default levels" << std::endl;
}

bool Planets::loadPlanets(JSONValue* planetDef, JSONValue* planetObj){
  if (planetDef && planetDef->HasChild("planets")){
    planets.clear();
    JSONValue *planetList = planetDef->Child("planets");

    unsigned count = planetList->CountChildren();

    planets.reserve(count);

    for(unsigned i = 0; i < count; ++i){
      std::string name = planetList->Child(i)->AsString();

      Vector2f loc = Vector2f(
          planetObj->ChildAtPath(name+".loc.x")->AsNumber(),
          planetObj->ChildAtPath(name+".loc.y")->AsNumber()
          );

      planets.push_back(Planet(
            planetObj->ChildAtPath(name+".sprite")->AsString(),
            planetObj->ChildAtPath(name+".mass")->AsNumber(),
            planetObj->ChildAtPath(name+".radius")->AsNumber(),
            loc)
          );
    }
    return true;
  }
  else return false;
}

void Planets::draw(){
  std::vector<Planet>::const_iterator it = planets.begin();
  while(it != planets.end()){
    it->draw();
    ++it;
  }
}

Vector2f Planets::accelerationFromPlanets(Vector2f loc){
  Vector2f retVal = Vector2f(0,0);

  // real value 0.00000000006674
  const double GRAV = 2000;

  double accelMag = 0;

  std::vector<Planet>::const_iterator it = planets.begin();
  while(it != planets.end()){

    Vector2f dist = it->getLocation() - loc;
    double r = dist.magnitude();
    double numer = GRAV*it->getMass();
    accelMag += numer/(r*r);

    double cosA = dist[0]/r;
    double sinA = dist[1]/r;

    retVal[0] += accelMag * cosA;
    retVal[1] += accelMag * sinA;

    ++it;
  }

  return retVal;
}

bool Planets::checkForCollision(const Drawable* obj){
  CollisionTest ct = CollisionTest::getInstance();
  std::vector<Planet>::const_iterator it = planets.begin();
  while(it != planets.end()){
    if(ct.check(CollisionTest::MidPoint,*it, *obj)){
      return true;
    }
    ++it;
  }
  return false;
}
