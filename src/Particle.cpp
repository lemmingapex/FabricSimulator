/* 
 * File:   Particle.cpp
 * Author: Scott
 * 
 * Created on June 24, 2013, 6:08 PM
 */


#include "Particle.h"

using namespace std;

Particle::Particle(Vector3f Position, bool Fixed) {
	position = Position;
	fixed = Fixed;
}

Particle::Particle(const Particle& orig) {
}

Particle::~Particle() {
}

