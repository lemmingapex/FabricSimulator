/* 
 * File:   ClothSystem.cpp
 * Author: Scott
 * 
 * Created on June 20, 2013, 1:01 PM
 */

#include "ClothSystem.h"
#include <cmath>

using namespace std;

/**
 * The main system of interest.
 * 
 * @param Size - How big is the cloth
 * @param fixStyle - Which particles are fixed?
 */
ClothSystem::ClothSystem(int Size, int fixStyle):GenericSystem() {
	this->size = Size;
	float restL = 0.1;
	float distance = 0.1;
	float springC = 200.0;
	float sqrt2 = sqrt(2);

	for (int i=0; i < this->size; i++) {
		for (int j=0; j < this->size; j++) {
			bool fixed = false;
			// Fix which particles?
			
			switch ( fixStyle%5 ) {
			case 0:
				fixed = edgesFixed(i, j);
				break;
			case 1:
				fixed = bowlFixed(i,j);
				break;
			case 2:
				fixed = cornersFixed(i, j);
				break;
			case 3:
				fixed = cornersFixed2(i, j);
				break;
			case 4:
				fixed = cornerFixed(i, j);
				break;
			default:
				break;
			}
			
			// particles
			particles.push_back(new Particle(Vector3f(j*distance, distance*sin(M_PI * ((double)i/(double)this->size) * 2.0), i*distance), fixed));
			
			// Structural Springs
			// Link(row,col) to Link(row+1,col)
			if (i < this->size-1) {
				int p0 = i*this->size + j;
				int p1 = (i+1)*this->size + j;
				springs.push_back(new Spring(p0, p1, restL, springC));
			}
			// Link(row,col) to Link(row,col+1)
			if (j < this->size-1) {
				int p0 = i*this->size + j;
				int p1 = i*this->size + (j+1);
				springs.push_back(new Spring(p0, p1, restL, springC));
			}

			// Shear Springs
			// Link(row,col) to Link(row+1,col+1)
			if (i < this->size-1 && j < this->size-1) {
				int p0 = i*this->size + j;
				int p1 = (i+1)*this->size + (j+1);
				springs.push_back(new Spring(p0, p1, restL*sqrt2, springC));
			}
			// Link(row,col) to Link(row+1,col-1)
			if (i < this->size-1 && j > 0) {
				int p0 = (i)*this->size + j;
				int p1 = (i+1)*this->size + (j-1);
				springs.push_back(new Spring(p0, p1, restL*sqrt2, springC));
			}

			// Flex Springs
			// Link(row,col) to Link(row+2,col)
			if (i < this->size-2) {
				int p0 = ((i)   * this->size + (j));
				int p1 = ((i+2) * this->size + (j));
				springs.push_back(new Spring(p0, p1, restL*2, springC));
			}
			// Link(row,col) to Link(row,col+2)
			if (j < this->size-2) {
				int p0 = (i) * this->size + (j);
				int p1 = (i) * this->size + (j+2);
				springs.push_back(new Spring(p0, p1, restL*2, springC));
			}
		}
	}
	for (int i=0; i < this->size-1; i++) {
		for (int j=0; j < this->size-1; j++) {
			//Faces
			//======================================================
			//Front - normals pointing forward
			int p0 = ((i+1)*this->size + (j+1));
			int p1 = ((i)*this->size + (j+1));
			int p2 = ((i+1)*this->size + (j));
			Face* fa1 = new Face(p0, p1, p2);
			faces.push_back(fa1);
			particles.at(p0)->addAdjacentFace(fa1);
			particles.at(p1)->addAdjacentFace(fa1);
			particles.at(p2)->addAdjacentFace(fa1);

			p0 = ((i)*this->size + (j+1));
			p1 = ((i)*this->size + (j));
			p2 = ((i+1)*this->size + (j));
			Face* fa2 = new Face(p0, p1, p2);
			faces.push_back(fa2);
			particles.at(p0)->addAdjacentFace(fa2);
			particles.at(p1)->addAdjacentFace(fa2);
			particles.at(p2)->addAdjacentFace(fa2);

			//Back - normals pointing backward
			p0 = ((i+1)*this->size + (j+1));
			p1 = ((i+1)*this->size + (j));
			p2 = ((i)*this->size + (j+1));
			Face* fa3 = new Face(p0, p1, p2);
			faces.push_back(fa3);

			p0 = ((i)*this->size + (j+1));
			p1 = ((i+1)*this->size + (j));
			p2 = ((i)*this->size + (j));
			Face* fa4 = new Face(p0, p1, p2);
			faces.push_back(fa4);
		}
	}

	init();
}

bool ClothSystem::bowlFixed(int i, int j) {
	bool fixed = false;
	if (i == 0 || j==0 || i == this->size-1 || j == this->size-1) {
		fixed = true;
	}
	return fixed;
}

bool ClothSystem::edgesFixed(int i, int j) {
	bool fixed = false;
	if ((i == 0 && j==0) || (i == 0 && j == this->size-1)) {
		fixed = true;
	}
	return fixed;
}

bool ClothSystem::cornersFixed(int i, int j) {
	bool fixed = false;
	if (i == this->size-1 || j == this->size-1) {
		fixed = true;
	}
	return fixed;
}

bool ClothSystem::cornersFixed2(int i, int j) {
	bool fixed = false;
	if ((i == this->size-1 || i == 0) && (j == this->size-1 || j==0)) {
		fixed = true;
	}
	return fixed;
}

bool ClothSystem::cornerFixed(int i, int j) {
	bool fixed = false;
	if ((i == this->size-1 && j==this->size-1)) {
		fixed = true;
	}
	return fixed;
}