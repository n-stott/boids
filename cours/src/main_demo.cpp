#include <iostream>
//#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <Eigen/Dense>  // inclue toute la librairie Eigen
#include <math.h>
#include <time.h>
#include <vector>
#include "camera_trackball.h"
#include "demo1.h"

const float FOVY = 60.0f;
const float NEARCLIP = 0.1f;
const float FARCLIP = 100.0f;
const int FPS = 30;
const int WINDOW_X = 700;
const int WINDOW_Y = 100;
// Inputs
int mouse_x = 0;
int mouse_y = 0;
int mouse_buttons[GLUT_NUM_MOUSE_BUTTONS];
int window_w = 800;
int window_h = 600;
// Camera
CameraTrackball camera;

std::vector<Demo*> demos;
unsigned int currentDemo = 0;

// Time management
float t = 0;
float last_t = 0;
float dt = 0;


void init(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
  
	// Init inputs
	for (int i = 0; i<GLUT_NUM_MOUSE_BUTTONS; ++i) {mouse_buttons[i] = GLUT_UP;}
  
	// Init camera
	camera.init({0.0f,0.0f,0.0f}, 10.0f);

	for(int i = 0; i < 1; ++i) {
		demos.push_back(new Demo1());
	}
	
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Camera setup
	camera.lookAt();
    
    //Display functions
    demos[currentDemo]->display();

	glutSwapBuffers();
}


void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, (GLfloat)w / (GLfloat)h, NEARCLIP, FARCLIP);
}


void processKeys(unsigned char key, int x, int y) {
	std::cout << key << std::endl;
	if (key == 'q') exit(0);
	switch (key) {
		case 'q': {
			exit(0);
		}
	}
}

void specialKey(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT: {
			demos[currentDemo]->prev();
			break;
		}
		case GLUT_KEY_RIGHT: {
			demos[currentDemo]->next();
			break;
		}
	}
}


void systemEvolution() {
	last_t = t;
	t = (float)glutGet(GLUT_ELAPSED_TIME);
	dt = (t - last_t)*0.001;

}


void mouseButton(int button, int state, int x, int y) {
  mouse_buttons[button] = state;
  
  // Update camera
  camera.zoom(mouse_buttons[3] - mouse_buttons[4]);
}

void mousePassiveMotion(int x, int y) {
  mouse_x = x; mouse_y = y;
}

void mouseMotion(int x, int y) {
  int mouse_dx = mouse_x - x; int mouse_dy = mouse_y - y;
  float dxn = static_cast<float>(mouse_dx) / static_cast<float>(window_w);
  float dyn = -static_cast<float>(mouse_dy) / static_cast<float>(window_h);
  mouse_x = x; mouse_y = y; 

  // Update camera
  if (mouse_buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN)  camera.rotate(dxn, dyn);
  if (mouse_buttons[GLUT_RIGHT_BUTTON] == GLUT_DOWN) camera.pan(dxn, dyn, 0.0f);
}

void timer(int v) {
  glutPostRedisplay();
  glutTimerFunc(1000 / FPS, timer, 0);
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) 
{
	
	glutInit(&argc, argv);
	glutInitWindowSize(window_w, window_h);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glViewport(0, 0, window_w, window_h);
	glutCreateWindow("Test window");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mousePassiveMotion);
	glutKeyboardFunc(processKeys);
	glutMotionFunc(mouseMotion);
	glutSpecialFunc(specialKey);


	glutIdleFunc(systemEvolution);
	glutTimerFunc(1000 / FPS, timer, 0);

	glutMainLoop();
	
	return 0;
}
