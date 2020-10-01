// --------------------------------------------------------------------------------------
//	OpenGL Triangle 
//	main.cpp
// --------------------------------------------------------------------------------------

#include "GLWindow.h"

// --------------------------------------------------------------------------------

int main(void)
{
	GLWindow		window(640, 480, "Window");
	if (window.Init() == false) exit(EXIT_FAILURE);

	GLfloat aspect;

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	aspect = (GLfloat)640 / 480;
	gluPerspective(45.0, aspect, 3.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(0.0f, 0.0f, -20.0f);

	while (window.IsRunning()) {
		glViewport(0, 0, window.m_nWidth, window.m_nHeight);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 0);
		glVertex3f(5.0f, 0, 0);
		glVertex3f(0, 5.0f, 0);
		glEnd();

		glFlush();

		window.Present();
	}
}
