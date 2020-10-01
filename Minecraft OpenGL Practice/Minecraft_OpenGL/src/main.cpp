// --------------------------------------------------------------------------------------
//	OpenGL 
//	Textures
// --------------------------------------------------------------------------------------

#include <math.h>
#include "GLWindow.h"
#include "Bitmap.h"
#include <gl/GL.h>
#include <gl/GLU.h>

// --------------------------------------------------------------------------------

float fx, fy;

int main(void)
{
	GLWindow		window(640, 480, "Cube");
	if (window.Init() == false) exit(EXIT_FAILURE);


	CBitmap* pCube = new CBitmap();
	pCube->LoadFromFile("Bitmap/cube2.jpg");

	GLuint		  cubeTexture;

	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pCube->m_nWidth, pCube->m_nHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pCube->m_pBytes);

	delete(pCube);


	GLfloat aspect;

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	aspect = (GLfloat)640 / 480;
	gluPerspective(45.0, aspect, 3.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	float vert[] = { -0.5,-0.5,0.5, 0.5,-0.5,0.5f, 0.5,0.5,0.5, -0.5,0.5,0.5, -0.5,-0.5,-0.5, 0.5,-0.5,-0.5f, 0.5,0.5,-0.5, -0.5,0.5,-0.5 };
	float uv[] = { 0.25,0.0, 0.5,0.0, 0.5,0.33, 0.25,0.33, 0.75,0.33, 0.75,0.66, 0.5,0.66, 0.5,0.33, 0.5,1.0, 0.25,1.0, 0.25,0.66, 0.5,0.66, 0.0,0.66, 0.0,0.33, 0.25,0.33, 0.25,0.66, 0.25,0.33, 0.5,0.33, 0.5,0.66, 0.25,0.66, 1.0,0.33, 1.0,0.66, 0.75,0.66, 0.75,0.33 };
	int		index[] = { 0,1,2,3, 1,5,6,2, 5,4,7,6, 4,0,3,7, 3,2,6,7, 1,0,4,5 };


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	glCullFace(GL_BACK);
	glTranslatef(0.0f, 0.0f, -20.0f);
	glScalef(4.0f, 4.0f, 4.0f);

	float fAngle = 0.0f;

	while (window.IsRunning()) {
		glViewport(0, 0, window.m_nWidth, window.m_nHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix();
		glRotatef(fAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(fAngle, 1.0f, 0.0f, 0.0f);
		fAngle += 0.5;

		glBegin(GL_QUADS);
		for (int i = 0; i < 24; i++) {
			glTexCoord2f(uv[i * 2], uv[i * 2 + 1]);
			glVertex3f(vert[index[i] * 3], vert[index[i] * 3 + 1], vert[index[i] * 3 + 2]);
		}
		glEnd();

		glPopMatrix();

		glFlush();

		window.Present();
	}

	return 0;
}
