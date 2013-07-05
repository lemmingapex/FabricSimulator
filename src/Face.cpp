/* 
 * File:   Face.cpp
 * Author: Scott
 * 
 * Created on June 30, 2013, 6:00 PM
 */

#include "Face.h"

Face::Face(int P0Index, int P1Index, int P2Index) {
	pIndex[0] = P0Index;
	pIndex[1] = P1Index;
	pIndex[2] = P2Index;
}

Face::Face(const Face& orig) {
}

Face::~Face() {
}



