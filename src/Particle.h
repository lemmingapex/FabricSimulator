/* 
 * File:   Particle.h
 * Author: Scott
 *
 * Created on June 24, 2013, 6:08 PM
 */

#ifndef PARTICLE_H
#define	PARTICLE_H

#include <vecmath.h>
#include <set>

#include "Face.h"
#include "GLIncludes.h"

using namespace std;

/**
 * Stores the absolute structure of a particle.
 * 
 * @param Position
 * @param Fixed
 */
class Particle {
public:
	Particle(Vector3f Position, bool Fixed=false);
	Particle(const Particle& orig);
	virtual ~Particle();

	bool isFixed() const {
		return fixed;
	}

	void setFixed(bool Fixed) {
		this->fixed = Fixed;
	}

	Vector3f getNormal() const {
		return normal;
	}

	void setNormal(Vector3f normal) {
		this->normal = normal;
	}

	Vector3f getPosition() const {
		return position;
	}

	void setPosition(Vector3f position) {
		this->position = position;
	}
	
	void draw();

	set<Face*> getAdjacentFaces() const {
		return adjacentFaces;
	}
	
	void addAdjacentFace(Face* adjacentFace) {
		this->adjacentFaces.insert(adjacentFace);
	}

protected:
	Vector3f position;
	bool fixed;
	Vector3f normal;
	
	/**
	 * Used for smooth shading
	 */
	set<Face*> adjacentFaces;

};

#endif	/* PARTICLE_H */

