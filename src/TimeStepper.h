/* 
 * File:   TimeStepper.h
 * Author: Scott
 * 
 * Created on June 24, 2013, 8:10 PM
 */

#ifndef TIMESTEPPER_H
#define TIMESTEPPER_H

#include <string>
#include "vecmath.h"
#include "GenericSystem.h"

/**
 * Abstract class.  Children integrate with different methods. 
 * 
 * @param particleSystem
 * @param stepSize
 */
class TimeStepper {
public:
	virtual void takeStep(GenericSystem* particleSystem, float stepSize)=0;
	virtual string name()=0;
};

class ForwardEuler:public TimeStepper {
	void takeStep(GenericSystem* particleSystem, float stepSize);
	string name(void){ return "ForwardEuler"; }
};

class Trapezoidal:public TimeStepper {
	void takeStep(GenericSystem* particleSystem, float stepSize);
	string name(void){ return "Trapezoidal"; }
};

class RungeKutta4:public TimeStepper {
	void takeStep(GenericSystem* particleSystem, float stepSize);
	string name(void){ return "RungeKutta4"; }
};

#endif