#include "planets.h"

Planets& Planets::getInstance(){
	static Planets instance;
	return instance;
}

Planets::Planets():planets(){
	JSONGamedata& jgdata = JSONGamedata::getInstance();
	if (jgdata.hasValue("planets")){
		JSONValue *planetList = jgdata.getValue("planets");
		
		unsigned count = planetList->CountChildren();

		planets.reserve(count);

		for(unsigned i = 0; i < count; ++i){
			std::string name = planetList->Child(i)->AsString();
			
			Vector2f loc = Vector2f(
				jgdata.getFloat(name+".loc.x"),
				jgdata.getFloat(name+".loc.y")
			);

			planets.push_back(Planet(
				name,
				jgdata.getFloat(name+".mass"),
				jgdata.getFloat(name+".radius"),
				loc)
			);
		}
	}
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
	const double GRAV = 100;

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
	// CollisionTest ct = CollisionTest::getInstance();
	// std::vector<Planet>::const_iterator it = planets.begin();
	// while(it != planets.end()){
	// 	if(ct.check(CollisionTest::Rectangular,*it,obj))
	// 		return true;
	// }
}
