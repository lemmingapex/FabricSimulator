#include "SimpleChain.h"
#include "Particle.h"
#include "Spring.h"

using namespace std;

SimpleChain::SimpleChain(unsigned int Size):GenericSystem() {
	particles.push_back(new Particle(Vector3f(1.0, 2.0, 1.0), true));
	for(unsigned int i=0; i<Size; i++) {
		particles.push_back(new Particle(randomPos(), randomFixed()));
		springs.push_back(new Spring(i, i+1, 0.25, 5.0));
	}
	init();
}

Vector3f SimpleChain::randomPos() {
	float size = 8.0;
	return Vector3f(((float(rand()) / RAND_MAX)*size) - size/2.0,((float(rand()) / RAND_MAX)*size) - size/2.0,((float(rand()) / RAND_MAX)*size) - size/2.0);
}

bool SimpleChain::randomFixed() {
	float prob = 0.7;
	if((float(rand()) / RAND_MAX) > prob) {
		return true;
	}
	return false;
}