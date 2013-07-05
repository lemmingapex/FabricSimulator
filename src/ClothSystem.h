/* 
 * File:   ClothSystem.h
 * Author: Scott
 * 
 * Created on June 20, 2013, 1:01 PM
 */

#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "GenericSystem.h"

using namespace std;

class ClothSystem: public GenericSystem {
public:
	ClothSystem(int C_SIZE, int fixStyle=0);
protected:
	int size;
	bool bowlFixed(int i, int j);
	bool edgesFixed(int i, int j);
	bool cornersFixed(int i, int j);
	bool cornersFixed2(int i, int j);
	bool cornerFixed(int i, int j);
};

#endif
