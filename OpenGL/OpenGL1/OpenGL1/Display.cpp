#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Display.h"
#include "GOManager.h"

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < game_objects.size(); i++) {
		GLfloat gl_x = (GLfloat)(game_objects.at(i))->x;
		GLfloat gl_y = (GLfloat)(game_objects.at(i))->y;
		glLineWidth(2.5);
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex3f(gl_x, gl_y + 0.1, 0.0);
		glVertex3f(gl_x + 0.1, gl_y - 0.1, 0.0);
		glVertex3f(gl_x - 0.1, gl_y - 0.1, 0.0);
		glEnd();
	}
	glutSwapBuffers();
}

Display::Display(int width, int height, const std::string&title) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("View");

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