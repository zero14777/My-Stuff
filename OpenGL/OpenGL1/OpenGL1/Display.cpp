#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Display.h"
#include "GOManager.h"

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);// scales image to fit window
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat l1Black[] = { 0.0, 0.0, 0.0, 1 };
	GLfloat l1Green[] = { 0.0, 1.0, 0.0, 1 };
	GLfloat l1White[] = { 1.0, 1.0, 1.0, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, l1Green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, l1Green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, l1White);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

	/*for (int i = 0; i < game_objects.size(); i++) {
		GLfloat gl_x = (GLfloat)(game_objects.at(i))->x;
		GLfloat gl_y = (GLfloat)(game_objects.at(i))->y;
		//glColor3f(0.0, 1.0, 0.0);replaced by lighting
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(gl_x, gl_y + 1, 0.0);
		glVertex3f(gl_x + 1, gl_y - 1, 0.0);
		glVertex3f(gl_x - 1, gl_y - 1, 0.0);
		glEnd();
	}*/

	for (int i = -90; i < 90; i++) {
		for (int j = -90; j < 90; j++) {
			glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(i*0.01, j*0.01, -.2);
			glVertex3f((i+1)*0.01, j*0.01, -.2);
			glVertex3f((i+1)*0.01, (j+1)*0.01, -.2);
			glVertex3f(i*0.01, (j+1)*0.01, -.2);
			glEnd();
		}
	}

	/*glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(1, 1, -.2);
	glVertex3f(1, -1, -.2);
	glVertex3f(-1, -1, -.2);
	glVertex3f(-1, 1, -.2);
	glEnd();*/

	glutSwapBuffers();
}

Display::Display(int width, int height, const std::string&title) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("View");

	//Initialization
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);//sets matrix transforms to affect the projection matrix
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	//Lighting
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//set intensity, color and position
	GLfloat l1Ambient[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat l1Diffuse[] = { 0.8, 0.8, 0.8, 1 };
	GLfloat l1Specular[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat l1position[] = { 0.5, 0.5, 0.0, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l1Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, l1position);
	//End Initialization

	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(render);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
	}

	glutMainLoop();
}

Display::~Display() {
	
}