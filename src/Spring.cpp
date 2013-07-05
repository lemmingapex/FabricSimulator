/* 
 * File:   Spring.cpp
 * Author: Scott
 * 
 * Created on June 24, 2013, 8:17 PM
 */

#include "Spring.h"

using namespace std;

Spring::Spring(int P0Index, int P1Index, float RestLength, float SpringConstant) {
	p0Index = P0Index;
	p1Index = P1Index;
	restLength = RestLength;
	springConstant = SpringConstant;
}

Spring::Spring(const Spring& orig) {
}

Spring::~Spring() {
}
