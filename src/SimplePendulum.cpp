#include "SimplePendulum.h"

using namespace std;

SimplePendulum::SimplePendulum():GenericSystem() {
	particles.push_back(new Particle(Vector3f(1.0, 2.0, 1.0), true));
	particles.push_back(new Particle(Vector3f(0.0, 1.0, 1.0)));;

	springs.push_back(new Spring(0, 1));
	
	init();
}

