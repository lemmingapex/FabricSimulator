/* 
 * File:   Face.h
 * Author: Scott
 *
 * Created on June 30, 2013, 6:00 PM
 */

#ifndef FACE_H
#define	FACE_H

#include "GLIncludes.h"

using namespace std;

/**
 * Store the graphical face info.  Connections.
 * 
 * @param P0Index
 * @param P1Index
 * @param P2Index
 */
class Face {
public:
	Face(int P0Index, int P1Index, int P2Index);
	Face(const Face& orig);
	virtual ~Face();
	
	int getP0Index() const {
		return pIndex[0];
	}

	void setP0Index(int P0Index) {
		pIndex[0] = P0Index;
	}

	int getP1Index() const {
		return pIndex[1];
	}

	void setP1Index(int P1Index) {
		pIndex[1] = P1Index;
	}
	
	int getP2Index() const {
		return pIndex[2];
	}

	void setP2Index(int P2Index) {
		pIndex[2] = P2Index;
	}

	void draw();
		
private:
	int pIndex[3];

};

#endif	/* FACE_H */

