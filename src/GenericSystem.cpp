/* 
 * File:   GenericSystem.cpp
 * Author: Scott
 * 
 * Created on June 24, 2013, 6:22 PM
 */

#include "GenericSystem.h"

using namespace std;

GenericSystem::GenericSystem() {
	// initialize forces
	mg = -1.9f;
	vDrag = 1.3f;
	wind = false;
	sinusoidal = false;
	degree = 0;
}

GenericSystem::GenericSystem(const GenericSystem& orig) {
}

GenericSystem::~GenericSystem() {
}

void GenericSystem::init() {
	// fill in code for initializing the state based on the of number particles
	for (unsigned int i = 0; i < particles.size(); i++) {
		// for this system, we care about the position and the force
		state.push_back(particles.at(i)->getPosition());
		state.push_back(Vector3f(0.0, 0.0, 0.0));
	}
}

// for a given state, evaluate f(X,t)
vector<Vector3f> GenericSystem::evalF(vector<Vector3f> state) {
	vector<Vector3f> f;
	
	// force from gravity and drag
	for (unsigned int s=0; s<state.size(); s++) {
		Vector3f derivF_v = state[2*s+1];
		f.push_back(derivF_v);

		Vector3f f_Gravity = Vector3f(0,mg,0);	//Gravity
		Vector3f f_Drag = -1*vDrag*state[2*s+1];	//Drag
		Vector3f f_Net = f_Gravity + f_Drag;	//Net Force
		f.push_back(f_Net);
	}

	// force from springs
	for (unsigned int i = 0; i < springs.size(); i++) {
		Spring* s = springs.at(i);
		
		int p0Index = s->getP0Index();
		int p1Index = s->getP1Index();
		
		float restLength = s->getRestLength();
		float springConstant = s->getSpringConstant();

		Vector3f p0 = state[2*p0Index];
		Vector3f p1 = state[2*p1Index];

		Vector3f p0_p1 = p0 - p1;

		Vector3f f_Spring_p0 = -1*springConstant*(p0_p1.abs()-restLength)*(p0_p1/(p0_p1.abs()));
		Vector3f f_Spring_p1 = -1* f_Spring_p0;

		f[2*p0Index + 1] += f_Spring_p0;
		f[2*p1Index + 1] += f_Spring_p1;
	}

	//Wind forces
	if (wind) {
		for (unsigned int i = 0; i < state.size(); i+=2) {
			float strength = 2.0*float(rand()) / RAND_MAX;
			
			float xDirectionAddition = (float(rand()) / RAND_MAX);
			float yDirectionAddition = (float(rand()) / RAND_MAX);
			float zDirectionAddition = (float(rand()) / RAND_MAX);
			
			f[2*i+1] += strength*Vector3f(-1.0 - xDirectionAddition, 0.0 + zDirectionAddition, -2.0 - yDirectionAddition);
		}
	}
	
	// Make necessary particles fixed and set position
	for (unsigned int k = 0; k < particles.size(); k++) {
		if (particles.at(k)->isFixed()) {
			f[2*k + 1] = Vector3f(0.0, 0.0, 0.0);
		}
	}

	//Sinusoidal Movement
	if (sinusoidal) {
		degree = (degree+1)%(2*360);
		float v = 1.15f * cos(degree*.5*M_PI/180.0);
		for (unsigned int i = 0; i < particles.size(); i++) {
			if (particles.at(i)->isFixed()) {
				f[2*i] = Vector3f(0.0f, 0.0f, v);
			}
		}
	}

	return f;
}

void GenericSystem::draw() {
	if(faces.size() == 0) {
		for (unsigned int i = 0; i < getState().size(); i+=2) {
			drawParticle(getState().at(i));
		}

		for (unsigned int i = 0; i < springs.size(); i++) {
			Spring* s = springs.at(i);
			drawSpring(getState().at(2*s->getP0Index()), getState().at(2*s->getP1Index()));			
		}
	} else {
		drawCloth();
	}

	// Ball
	if (true) {
		drawBall();
	}
}
	
void GenericSystem::drawParticle(Vector3f position) {
	GLfloat particleColor[] = {0.51f, 0.48f, 0.71f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, particleColor);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(0.07, 10.0, 10.0);
	glPopMatrix();
}

void GenericSystem::drawSpring(Vector3f p0pos, Vector3f p1pos) {
	GLfloat lineColor[] = {1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, lineColor);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(p0pos.x(), p0pos.y(), p0pos.z());
	glVertex3f(p1pos.x(), p1pos.y(), p1pos.z());
	glEnd();
}

void GenericSystem::drawCloth() {
	GLfloat clothColor[] = {0.51f, 0.48f, 0.71f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clothColor);
	
	vector<Vector3f> particleNorms;
	// calc particle norms
	for (unsigned int i = 0; i < particles.size(); i++) {
		set<Face*> adjacentFaces = particles.at(i)->getAdjacentFaces();
		set<Face*>::iterator pos;
		Vector3f norm = Vector3f(0,0,0);
		for(pos = adjacentFaces.begin(); pos != adjacentFaces.end(); ++pos) {
			Face *af = *pos;
			Vector3f av0 = getState().at(2*(af->getP0Index()));
			Vector3f av1 = getState().at(2*(af->getP1Index()));
			Vector3f av2 = getState().at(2*(af->getP2Index()));
			norm += Vector3f::cross(av1-av0, av2-av0);
		}

		norm = norm.normalized();
		particleNorms.push_back(norm);
	}

	for (unsigned int f = 0; f < faces.size(); f++) {
		Face* face = faces[f];
		Vector3f v0 = getState().at(2*face->getP0Index());
		Vector3f n0 = particleNorms.at(face->getP0Index());

		Vector3f v1 = getState().at(2*face->getP1Index());
		Vector3f n1 = particleNorms.at(face->getP1Index());

		Vector3f v2 = getState().at(2*face->getP2Index());
		Vector3f n2 = particleNorms.at(face->getP2Index());

		if((f+1)%4 == 0 || (f+2)%4 == 0 ) {
			n0 = -1.0*n0;
			n1 = -1.0*n1;
			n2 = -1.0*n2;
		}

		//Vector3f n = Vector3f::cross(v1-v0, v2-v0).normalized();
		glBegin(GL_TRIANGLES);
		//glNormal3f(n[0], n[1], n[2]);
		glNormal3f(n0[0], n0[1], n0[2]);
		glVertex3f(v0[0], v0[1], v0[2]);
		glNormal3f(n1[0], n1[1], n1[2]);
		glVertex3f(v1[0], v1[1], v1[2]);
		glNormal3f(n2[0], n2[1], n2[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
		glEnd();
	}
}

void GenericSystem::drawBall(Vector3f location) {
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.79f, 0.62f, 0.29f);
	float radBall = 0.8f;
	float epsilon = 0.12f;
	glPushMatrix();
	glTranslatef(location.x(), location.y(), location.z());
	glutSolidSphere(radBall, 30.0, 30.0);
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
	for (unsigned int n = 0; n < particles.size(); n++) {
		if ((getState().at(2*n) - location).abs() <= (radBall + epsilon)) {
			state[2*n] = (location + (radBall + epsilon) * (getState().at(2*n) - location).normalized());
		}
	}
}