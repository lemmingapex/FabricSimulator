/* 
 * File:   TimeStepper.cpp
 * Author: Scott
 * 
 * Created on June 24, 2013, 8:10 PM
 */

#include "TimeStepper.h"

/**
 * Unstable method
 * 
 * @param particleSystem
 * @param stepSize
 */
void ForwardEuler::takeStep(GenericSystem* particleSystem, float stepSize) {
	vector<Vector3f> newX;

	vector<Vector3f> currentX = particleSystem->getState();
	vector<Vector3f> f_X = particleSystem->evalF(currentX);

	for (unsigned int i=0; i<currentX.size(); i++) {
		Vector3f x = currentX[i] + stepSize*f_X[i];
		newX.push_back(x);
	}

	particleSystem->setState(newX);
}

/**
 * Unstable method
 * 
 * @param particleSystem
 * @param stepSize
 */
void Trapezoidal::takeStep(GenericSystem* particleSystem, float stepSize) {
	vector<Vector3f> newX;

	vector<Vector3f> currentX = particleSystem->getState();
	vector<Vector3f> f0 = particleSystem->evalF(currentX);
	
	vector<Vector3f> f1_x;
	for (unsigned int i=0; i<f0.size(); i++) {
		Vector3f X_each = currentX[i] + (f0[i]*stepSize);
		f1_x.push_back(X_each);
	}

	vector<Vector3f> f1 = particleSystem->evalF(f1_x);

	for (unsigned int i=0; i<currentX.size(); i++) {
		Vector3f eachX = currentX[i] + ((f0[i] + f1[i])*(stepSize/2.0));
		newX.push_back(eachX);
	}

	particleSystem->setState(newX);
}

/**
 * Fairly stable method
 * 
 * @param particleSystem
 * @param stepSize
 */
void RungeKutta4::takeStep(GenericSystem* particleSystem, float stepSize) {
	vector<Vector3f> newX;

	vector<Vector3f> k1_x = particleSystem->getState();
	vector<Vector3f> k1 = particleSystem->evalF(k1_x);
	
	vector<Vector3f> k2_x;
	for (unsigned int i=0; i<k1.size(); i++) {
		k2_x.push_back(k1_x[i] + (k1[i]*(stepSize/2.0)));
	}
	vector<Vector3f> k2 = particleSystem->evalF(k2_x);

	vector<Vector3f> k3_x;
	for (unsigned int i=0; i<k2.size(); i++) {
		k3_x.push_back(k1_x[i] + (k2[i]*(stepSize/2.0)));
	}
	vector<Vector3f> k3 = particleSystem->evalF(k3_x);
	
	vector<Vector3f> k4_x;
	for (unsigned int i=0; i<k3.size(); i++) {
		k4_x.push_back(k1_x[i] + (k3[i]*stepSize));
	}
	vector<Vector3f> k4 = particleSystem->evalF(k4_x);
	
	for (unsigned int i=0; i<k1_x.size(); i++) {
		Vector3f x = k1_x[i] + (stepSize/6.0)*(k1[i] + 2.0*k2[i] + 2*k3[i] + k4[i]);
		newX.push_back(x);
	}

	particleSystem->setState(newX);
}