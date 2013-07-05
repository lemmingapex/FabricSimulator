#define _USE_MATH_DEFINES

#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <vecmath.h>

#include "GLIncludes.h"
#include "Camera.h"
#include "GenericSystem.h"
#include "TimeStepper.h"
#include "SimplePendulum.h"
#include "SimpleChain.h"
#include "ClothSystem.h"

using namespace std;

namespace {
	GenericSystem *system;

	// This thing can add dude
	TimeStepper * integrator;
	
	// how fast is time?  System should be stable in most cases around .04
	float stepSize;
	
	// resoultion of the cloth or chain
	int systemSize;
	
	// This is the camera
	Camera camera;

	// These are state variables for the UI
	bool g_mousePressed = false;
	
	int fixStyle = 0;

	/**
	 * Prints controls
     */
	void printHelp() {
		printf("\nControls\n");
		printf("=========\n");
		printf("click and drag:\t Move camera.\n");
		printf("right click and drag:\t Zoom camera.\n");
		printf("space:\t Reset camera.\n");
		printf("f:\t Reset fabric system.\n");
		printf("w:\t Toggle wind.\n");
		printf("t:\t Toggle fixed point style. Fabric only.\n");
		printf("s:\t Sinusoidal movement.\n");
		printf("c:\t Reset chain system.\n");
		printf("Up arrow:\t Increase system size.\n");
		printf("Down arrow:\t Decrease system size.\n");
	}
	
	/**
	 * initialize your particle systems
	 * 
     * @param argc
     * @param argv
     */
	void initSystem(int argc, char * argv[]) {
		// seed the random number generator with the current time
		srand( time( NULL ) );
		
		// default integrator
		char integratorInput = 'R';
		// default step size
		string stepSizeInput = ".04";
		
		string systemSizeInput = "15";
		
		if (argc > 4) {
			printf("Usage:\n%s [Step Size] [System Size] [Integrator]\n", argv[0]);
			printf("Example usage:\n%s %s %s RungeKutta4\n", argv[0], stepSizeInput.c_str(), systemSizeInput.c_str());
			exit(0);
		}
		if(argc > 1) {
			stepSizeInput = argv[1];
		}
		if(argc > 2) {
			systemSizeInput = argv[2];
		}
		if(argc > 3) {
			integratorInput = toupper(argv[3][0]);
		}

		printf("Inputs\n");
		printf("======\n");
		
		stepSize = atof(stepSizeInput.c_str());
		printf("Step size: %f\n", stepSize);

		systemSize = atoi(systemSizeInput.c_str());
		printf("System size: %i\n", systemSize);
		
		if (integratorInput == 'F') {
			integrator = new ForwardEuler();
		} else if (integratorInput == 'T') {
			integrator = new Trapezoidal();
		} else {
			integrator = new RungeKutta4();
		}
		printf("Integrator: %s\n", integrator->name().c_str());
		
		printHelp();
		
		system = new ClothSystem(systemSize, fixStyle);
	}

	/**
	 * Draws the floor.  Systems do not interact with the floor
     */
	void drawFloor() {
		GLfloat floorColor[] = {0.76f, 0.32f, 0.31f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
		glPushMatrix();
		glTranslatef(0.0f,-5.0f,0.0f);
		glScaled(30.0f,0.1f,30.0f);
		glutSolidCube(1);
		glPopMatrix();
	}
	
	/**
	 * Draws the coordinate axes when you're rotating.
     */
	void drawAxes() {
		if( g_mousePressed ) {
			glPushMatrix();
			Vector3f eye = camera.GetCenter();
			glTranslatef( eye[0], eye[1], eye[2] );
			// Save current state of OpenGL
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			// This is to draw the axes when the mouse button is down
			glDisable(GL_LIGHTING);
			glLineWidth(3);
			
			glPushMatrix();
			glScaled(5.0, 5.0, 5.0);
			glBegin(GL_LINES);
			glColor3f(1.0, 0.5, 0.5);
			glVertex3f(0, 0, 0);
			glVertex3f(1, 0, 0);
			glColor3f(0.5, 1.0 ,0.5);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1, 0);
			glColor3f(0.5, 0.5, 1.0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1);

			glColor3f(0.5, 0.5, 0.5);
			glVertex3f(0, 0, 0);
			glVertex3f(-1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, -1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, -1);

			glEnd();
			glPopMatrix();

			glPopAttrib();
			glPopMatrix();
		}
	}
	
	/**
	 * Draws stuff
     */
	void drawAll() {
		system->draw();
		drawFloor();
		drawAxes();
	}
	
	/**
	 * Make opengl happy
     */
	void drawScene() {
		// Clear the rendering window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		// Light color (RGBA)
		GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
		GLfloat Lt0pos[] = {3.0,4.0,5.0,1.0};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
		glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

		glLoadMatrixf( camera.viewMatrix() );

		// Draw stuff we care about
		drawAll();

		// Dump the image to the screen.
		glutSwapBuffers();
	}

	/**
	 *  Initialize OpenGL's rendering modes
	 */
	void initRendering() {
		glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
		glEnable(GL_LIGHTING);     // Enable lighting calculations
		glEnable(GL_LIGHT0);       // Turn on light #0.

		glEnable(GL_NORMALIZE);

		// Setup polygon drawing
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Clear to black
		glClearColor(0,0,0,1);
	}

	/**
	 * Put the camera back
     */
	void resetCamera() {
		Matrix4f eye = Matrix4f::identity();
		eye = Matrix4f::rotateY(-M_PI/4);
		eye = Matrix4f::rotateX(M_PI/12) * eye;
		camera.SetDistance(11);
		camera.SetRotation( eye );
		camera.SetCenter( Vector3f::ZERO );
	}
	
	/**
	 * handle "Normal" key press
	 * 
     * @param key
     * @param x
     * @param y
     */
	void keyboardFunc( unsigned char key, int x, int y ) {
        switch ( key ) {
			case 27: // Escape key
				exit(0);
				break;
			case 'w':
				printf("Wind toggle\n");
				system->windToggle();
				break;
			case 's':
				printf("Sinusoidal toggle\n");
				system->sinusoidalToggle();
				break;
			case 'f':
				printf("Fabric\n");
				system = new ClothSystem(systemSize, fixStyle);
				break;
			case 'c':
				printf("Chain\n");
				system = new SimpleChain(systemSize);
				break;
			case 't':
				printf("Fixed Style, fabric only\n");
				fixStyle++;
				system = new ClothSystem(systemSize, fixStyle);
				break;
			case ' ': {
				resetCamera();
				break;
			}
			default:
				break;
			}

		glutPostRedisplay();
	}
	
	/**
	 * Handles the arrow keys.
	 * 
     * @param key
     * @param x
     * @param y
     */
	void specialFunc( int key, int x, int y ) {
		switch ( key ) {
		case 100: // left arrow;
			break;
		case 101: // up arrow
			systemSize++;
			printf("System Size: %i\n", systemSize);
			break;
		case 102: // right arrow
			break;
		case 103: // down arrow
			systemSize = max(1, systemSize-1);
			printf("System Size: %i\n", systemSize);
			break;
		}
	}

	/**
	 * When mouse button is pressed.
	 * 
     * @param button
     * @param state
     * @param x
     * @param y
     */
	void mouseFunc(int button, int state, int x, int y) {
		if (state == GLUT_DOWN) {
			g_mousePressed = true;

			switch (button) {
			case GLUT_LEFT_BUTTON:
				camera.MouseClick(Camera::LEFT, x, y);
				break;
			case GLUT_MIDDLE_BUTTON:
				camera.MouseClick(Camera::MIDDLE, x, y);
				break;
			case GLUT_RIGHT_BUTTON:
				camera.MouseClick(Camera::RIGHT, x, y);
			default:
				break;
			}
		} else {
			camera.MouseRelease(x, y);
			g_mousePressed = false;
		}
		glutPostRedisplay();
	}

	/**
	 * Called when mouse is moved while left button pressed
	 * 
     * @param x
     * @param y
     */
	void motionFunc(int x, int y) {
		camera.MouseDrag(x,y);
		glutPostRedisplay();
	}
	
	/**
	 * Called when the window is resized
	 * 
     * @param w width of the window in pixels
     * @param h height of the window in pixels
     */
	void reshapeFunc(int w, int h) {
		camera.SetDimensions(w,h);
		camera.SetViewport(0,0,w,h);
		camera.ApplyViewport();

		// Set up a perspective view, with square aspect ratio
		glMatrixMode(GL_PROJECTION);

		camera.SetPerspective(50);
		glLoadMatrixf( camera.projectionMatrix() );
	}
	
	/**
	 * Steps the system
     */
	void stepSystem() {
		const float h = stepSize;
		integrator->takeStep(system, h);
	}
	
	void timerFunc(int t) {
		stepSystem();
		glutPostRedisplay();
		glutTimerFunc(t, &timerFunc, t);
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	// We're going to animate it, so double buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	int w = 800;
	int h = 600;
	// Initial parameters for window position and size
	glutInitWindowPosition(60, 60);
	glutInitWindowSize(w, h);

	camera.SetDimensions(w, h);
	resetCamera();

	glutCreateWindow("Fabric Simulation");

	// Initialize OpenGL parameters.
	initRendering();

	// Setup particle system
	initSystem(argc, argv);

	// Set up callback functions for key presses
	// Handles "normal" ascii symbols
	glutKeyboardFunc(keyboardFunc);
	// Handles "special" keyboard keys
	glutSpecialFunc(specialFunc);

	// Set up callback functions for mouse
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);

	// Set up the callback function for resizing windows
	glutReshapeFunc(reshapeFunc);

	// Call this whenever window needs redrawing
	glutDisplayFunc(drawScene);

	// Trigger timerFunc every 20 msec
	glutTimerFunc(20, timerFunc, 20);

	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop();
	return 0;
}
