#include <iostream>
#include "GL/freeglut.h"
#include "Constants.h"
#include "Stage.h"
#include "Platform.h"

using namespace std;

clock_t start_t = clock();
clock_t end_t;
vector<Stage> stages;

void initialize() {
	Stage stage1(1);
	vector<string> platformInfo;
	platformInfo.push_back("¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á¢Ã¢Ã    ¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã    ¢Ã¢Ã¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á¢Ã¢Ã    ¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã    ¢Ã¢Ã¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á¢Ã¢Ã    ¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã¢Ã    ¢Ã¢Ã¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á                                                ¡á¡á");
	platformInfo.push_back("¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á");

	stage1.setStagePlatform(platformInfo);
	stages.push_back(stage1);
	platformInfo.clear();
}


void idle() {
	end_t = clock();
	if ((float)(end_t - start_t) > 1000 / 30.0f) {

		start_t = end_t;
	}

	glutPostRedisplay();
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);
	//gluOrtho2D(-boundaryX, boundaryX, -boundaryY, boundaryY);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	stages[0].draw();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Bubble Bobble");
	initialize();

	// register callbacks
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}