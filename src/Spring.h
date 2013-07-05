/* 
 * File:   Spring.h
 * Author: Scott
 *
 * Created on June 24, 2013, 8:17 PM
 */

#ifndef SPRING_H
#define	SPRING_H

#include "Particle.h"
#include "GLIncludes.h"

using namespace std;

/**
 * Stores the absolute structure of a spring.
 * 
 * @param P0Index
 * @param P1Index
 * @param RestLength
 * @param SpringConstant
 */
class Spring {
public:
	Spring(int P0Index, int P1Index, float RestLength=1.0, float SpringConstant=2.0);
	Spring(const Spring& orig);
	virtual ~Spring();
	
	int getP0Index() const {
		return p0Index;
	}

	void setP0Index(int P0Index) {
		p0Index = P0Index;
	}

	int getP1Index() const {
		return p1Index;
	}

	void setP1Index(int P1Index) {
		p1Index = P1Index;
	}

	float getRestLength() const {
		return restLength;
	}

	void setRestLength(float RestLength) {
		restLength = RestLength;
	}

	float getSpringConstant() const {
		return springConstant;
	}

	void setSpringConstant(float SpringConstant) {
		springConstant = SpringConstant;
	}
		
private:
	int p0Index;
	int p1Index;
	float restLength;
	float springConstant;

};

#endif	/* SPRING_H */

