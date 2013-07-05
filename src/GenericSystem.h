/* 
 * File:   GenericSystem.h
 * Author: Scott
 *
 * Created on June 24, 2013, 6:22 PM
 */

#ifndef GENERICSYSTEM_H
#define	GENERICSYSTEM_H

#include "GLIncludes.h"
#include "Particle.h"
#include "Spring.h"
#include "Face.h"
#include <cmath>
#include <vecmath.h>
#include <vector>

using namespace std;

/**
 * Handles much of the interaction and communication in the systems
 */
class GenericSystem {
public:
	GenericSystem();
	GenericSystem(const GenericSystem& orig);
	virtual ~GenericSystem();

	void init();
	
	vector<Vector3f> getState() {
		return state;
	}

	void setState(const vector<Vector3f> & State) {
		state = State;
	}
	
	void windToggle() {
		wind = !wind;
	}
	
	void sinusoidalToggle() {
		sinusoidal = !sinusoidal;
	}
	
	vector<Vector3f> evalF(vector<Vector3f> state);

	void draw();

	void drawParticle(Vector3f position);
	
	void drawSpring(Vector3f p0pos, Vector3f p1pos);
	
	void drawCloth();
	
	void drawBall(Vector3f location = Vector3f(0.5f, -1.5f, 0.0f));
	
	vector<Particle*> getParticles() const {
		return particles;
	}

	void setParticles(vector<Particle*> particles) {
		this->particles = particles;
	}

	
protected:
	// stores the state of the system, position at even indexes and force at odd indexes
	vector<Vector3f> state;
	
	// Spring ( first particle index, second particle index, restLength, springConstant)
	vector<Spring*> springs;

	// Particle ( Vector3f pos, bool fixed in space?)
	vector<Particle*> particles;

	// first particle index, second particle index, third particle index
	vector<Face*> faces;

	//Force Constants
	float mg;
	float vDrag;
	int degree;
	bool wind;
	bool sinusoidal;
};

#endif	/* GENERICSYSTEM_H */

