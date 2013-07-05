#ifndef SIMPLECHAIN_H
#define SIMPLECHAIN_H

#include "GenericSystem.h"

using namespace std;

class SimpleChain: public GenericSystem {
public:
	SimpleChain(unsigned int Size);
private:
	Vector3f randomPos();
	bool randomFixed();
};

#endif