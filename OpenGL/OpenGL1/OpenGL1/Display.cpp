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

	glEnable(GL_DEPTH_TEST);

	GLfloat Black[] = { 0.0, 0.0, 0.0, 1 };
	GLfloat Red[] = { 1.0, 0.0, 0.0, 1 };
	GLfloat Green[] = { 0.0, 1.0, 0.0, 1 };
	GLfloat Blue[] = { 0.0, 0.0, 1.0, 1 };
	GLfloat White[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat LowAmbient[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat HighAmbient [] = { 1.0, 1.0, 1.0, 1 };

	for (int iIndex = 0; iIndex < 2; ++iIndex) {
		if (iIndex == 0) {
			glShadeModel(GL_FLAT);
		}
		else {
			glShadeModel(GL_SMOOTH);
		}

		glMaterialfv(GL_FRONT, GL_AMBIENT, Green);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Green);
		glMaterialfv(GL_FRONT, GL_SPECULAR, White);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, LowAmbient);

		// Sphere
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(-0.5 + (float)iIndex, 0.5, -2.0);
		glutSolidSphere(0.5, 20, 20);
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_DIFFUSE, Black);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Black);
		glLightfv(GL_LIGHT0, GL_AMBIENT, HighAmbient);

		// Triangle
		glBegin(GL_TRIANGLES);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Red);
		glVertex3f(-0.5 + (float)iIndex, 0.0, -0.2);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Green);
		glVertex3f(-0.933 + (float)iIndex, -0.75, -0.2);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Blue);
		glVertex3f(-0.067 + (float)iIndex, -0.75, -0.2);
		glEnd();
	}

	/*for (int i = 0; i < game_objects.size(); i++) { GO's
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
	glOrtho(-1.0, 1.0, -1.0, 1.0, -3.0, 3.0);

	//Lighting
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//set intensity, color and position
	GLfloat l1Ambient[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat l1Diffuse[] = { 0.8, 0.8, 0.8, 1 };
	GLfloat l1Specular[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat l1position[] = { 0.0, 1.0, -0.5, 1 };
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