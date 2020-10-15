#include <math.h>
#include "GLWindow.h"
#include "Bitmap.h"
#include "GL/glew.h"
#include "PerlinNoise.h"
#include "Input.h"

using namespace std;

// --------------------------------------------------------------------------------

#define VERTICES		24
#define INDICES			36

#define BUFFER_OFFSET(i) ((void*)(i))

GLuint			vbo = 0;			// Vertex Buffer Object
GLuint			ibo = 0;			// Index Buffer Object

struct MyVertex
{
	float x, y, z;        // Vertex
	float nx, ny, nz;     // Normal
	float u, v;					// UV
};

MyVertex vertex[VERTICES] = {
{0.0,0.0,0.0 ,0.0,0.0,1.0 ,0.0,0.33},				// front	
{1.0,0.0,0.0 ,0.0,0.0,1.0 ,0.0,0.66},
{0.0,1.0,0.0 ,0.0,0.0,1.0 ,0.25,0.33},
{1.0,1.0,0.0 ,0.0,0.0,1.0 ,0.25,0.66},

{1.0,0.0,0.0 ,1.0,0.0,0.0 ,0.25,1.0},				// right
{1.0,0.0,-1.0 ,1.0,0.0,0.0 ,0.5,1.0},
{1.0,1.0,0.0 ,1.0,0.0,0.0 ,0.25,0.66},
{1.0,1.0,-1.0 ,1.0,0.0,0.0 ,0.5,0.66},

{1.0,0.0,-1.0 ,0.0,0.0,-1.0 ,0.75,0.33},			// back
{0.0,0.0,-1.0 ,0.0,0.0,-1.0 ,0.75,0.66},
{1.0,1.0,-1.0 ,0.0,0.0,-1.0 ,0.5,0.33},
{0.0,1.0,-1.0 ,0.0,0.0,-1.0 ,0.5,0.66},

{0.0,0.0,-1.0 ,-1.0,0.0,1.0 ,0.25,0.0},			// left
{0.0,0.0,0.0 ,-1.0,0.0,1.0 ,0.5,0.0},
{0.0,1.0,-1.0 ,-1.0,0.0,1.0 ,0.25,0.33},
{0.0,1.0,0.0 ,-1.0,0.0,1.0 ,0.5,0.33},

{0.0,1.0,0.0 ,0.0,1.0,0.0 ,0.25,0.66},			// top
{1.0,1.0,0.0 ,0.0,1.0,0.0 ,0.5,0.66},
{0.0,1.0,-1.0 ,0.0,1.0,0.0 ,0.25,0.33},
{1.0,1.0,-1.0 ,0.0,1.0,0.0 ,0.5,0.33},

{0.0,0.0,0.0 ,0.0,-1.0,1.0 ,1.0,0.66},				// bottom
{1.0,0.0,0.0 ,0.0,-1.0,1.0 ,1.0,0.33},
{0.0,0.0,-1.0 ,0.0,-1.0,1.0 ,0.75,0.66},
{1.0,0.0,-1.0 ,0.0,-1.0,1.0 ,0.75,0.33} };


unsigned short indices[INDICES];

// --------------------------------------------------------------------------------


bool	bMouseDown = false;
double  fStartX;
double  fStartY;
double  fHor;
double  fVer;

static void MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (bMouseDown) {
		fHor += (xpos - fStartX) / 1250.0;
		fStartX = xpos;
		fVer += (ypos - fStartY) / 1250.0;
		fStartY = ypos;
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &fStartX, &fStartY);
			bMouseDown = true;
		}
		else {
			bMouseDown = false;
		}
	}
}

// --------------------------------------------------------------------------------

int main(void)
{
	GLWindow window(640, 480, "Minecraft Project");
	if (window.Init() == false) exit(EXIT_FAILURE);

	glfwSetCursorPosCallback(window.m_Window, MousePosCallback);
	glfwSetMouseButtonCallback(window.m_Window, MouseButtonCallback);

	GLuint	skyTexture;

	GLuint	cubeTexture;
	GLfloat aspect = (GLfloat)640 / 480;

	GLfloat angle = 0.0f;

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_FLAT);

	// The Texture

	CBitmap* pBitmap = new CBitmap();
	pBitmap->LoadFromFile("Bitmap/cube2.jpg");

	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);					// GL_CLAMP,GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		// GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pBitmap->m_nWidth, pBitmap->m_nHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBitmap->m_pBytes);

	delete(pBitmap);

	// Create the Vertex Buffer Object

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * VERTICES, &vertex[0].x, GL_STATIC_DRAW);

	// Create the Index Buffer Object

	indices[0] = 0;			// front
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;

	indices[6] = 4;			// right
	indices[7] = 5;
	indices[8] = 6;
	indices[9] = 5;
	indices[10] = 7;
	indices[11] = 6;

	indices[12] = 8;		// back
	indices[13] = 9;
	indices[14] = 10;
	indices[15] = 9;
	indices[16] = 11;
	indices[17] = 10;

	indices[18] = 12;		// left
	indices[19] = 13;
	indices[20] = 14;
	indices[21] = 13;
	indices[22] = 15;
	indices[23] = 14;

	indices[24] = 16;		// top
	indices[25] = 17;
	indices[26] = 18;
	indices[27] = 17;
	indices[28] = 19;
	indices[29] = 18;

	indices[30] = 20;		// bottomn
	indices[31] = 21;
	indices[32] = 22;
	indices[33] = 21;
	indices[34] = 23;
	indices[35] = 22;

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * INDICES, indices, GL_STATIC_DRAW);

	// Describe the structur of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The starting point of the VBO, for the vertices

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(12));      // The starting point of normals, 12 bytes away

	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(24)); // The starting point of texcoords, 24 bytes away

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generation of a map via PerlinNoise

	// Define the dimension of the grid

	unsigned int m_nWidth = 128;
	unsigned int m_nHeight = 128;

	unsigned char* m_pData = (unsigned char*)malloc(m_nWidth * m_nHeight);

	unsigned int seed = 120;
	PerlinNoise pn(seed);

	unsigned int inc = 0;

	// skybox
	CBitmap* pSky = new CBitmap();
	pSky->LoadFromFile("Bitmap/DaylightBox.png");

	glGenTextures(1, &skyTexture);
	glBindTexture(GL_TEXTURE_2D, skyTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pSky->m_nWidth, pSky->m_nHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pSky->m_pBytes);
	delete(pSky);

	float	vert[] = { -0.5,-0.5,0.5, 0.5,-0.5,0.5f, 0.5,0.5,0.5, -0.5,0.5,0.5, -0.5,-0.5,-0.5, 0.5,-0.5,-0.5f, 0.5,0.5,-0.5, -0.5,0.5,-0.5 };
	float	uv[] = { 0.25,0.0, 0.5,0.0, 0.5,0.33, 0.25,0.33, 0.75,0.33, 0.75,0.66, 0.5,0.66, 0.5,0.33, 0.5,1.0, 0.25,1.0, 0.25,0.66, 0.5,0.66, 0.0,0.66, 0.0,0.33, 0.25,0.33, 0.25,0.66, 0.25,0.33, 0.5,0.33, 0.5,0.66, 0.25,0.66, 1.0,0.33, 1.0,0.66, 0.75,0.66, 0.75,0.33 };
	int		index[] = { 0,1,2,3, 1,5,6,2, 5,4,7,6, 4,0,3,7, 3,2,6,7, 1,0,4,5 };
	float	uvsky[] = { 0.0,0.33, 0.25,0.33, 0.25,0.66, 0.0,0.66, 0.25,0.33, 0.5,0.33, 0.5,0.66, 0.25,0.66, 0.5,0.33, 0.75,0.33, 0.75,0.66, 0.5,0.66, 0.75,0.33, 1.0,0.33, 1.0,0.66, 0.75,0.66, 0.25,1.0, 0.25,0.66, 0.5,0.66, 0.5,1.0, 0.25,0.0, 0.5,0.0, 0.5,0.33, 0.25,0.33 };

	// Fill the grid with values from the noise function

	for (unsigned int i = 0; i < m_nHeight; ++i) {     // y
		for (unsigned int j = 0; j < m_nWidth; ++j) {  // x
			double x = (double)j / ((double)m_nWidth);
			double y = (double)i / ((double)m_nHeight);

			double n = pn.noise(5 * x, 5 * y, 0.8);

			m_pData[inc] = (unsigned char)floor(26 * n);

			inc++;
		}
	}

	while (window.IsRunning()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, aspect, 3.0, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0f, 0.8f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


		// looking
		//float px = sin(fHor) * cos(fVer);
		//float py = sin(fVer);
		//float pz = cos(fHor) * cos(fVer);
		//gluLookAt(0.0f, 0.0f, 0.0f, px, py, pz, 0.0f, 1.0f, 0.0f);



		//glPushMatrix();

		// Assign the Texture

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -56.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);		// Rotate around the y axis

	    // skybox
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(1100.0f, 1100.0f, 1100.0f);
		glCullFace(GL_FRONT);

		glBindTexture(GL_TEXTURE_2D, skyTexture);
		glBegin(GL_QUADS);
		for (int i = 0; i < 24; i++) {
			glTexCoord2f(uvsky[i * 2], uvsky[i * 2 + 1]);
			glVertex3f(vert[index[i] * 3], vert[index[i] * 3 + 1], vert[index[i] * 3 + 2]);
		}
		glEnd();

		glScalef(1.0f / 1100.0f, 1.0f / 1100.0f, 1.0f / 1100.0f);
		// skybox end

		// cube test 
		glTranslatef(-0.5f, -10.0f, -5.0f);		
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(10.0f, 10.0f, 10.0f);
		glCullFace(GL_BACK);

		glBindTexture(GL_TEXTURE_2D, skyTexture);
		glBegin(GL_QUADS);
		for (int i = 0; i < 24; i++) {
			glTexCoord2f(uvsky[i * 2], uvsky[i * 2 + 1]);
			glVertex3f(vert[index[i] * 3], vert[index[i] * 3 + 1], vert[index[i] * 3 + 2]);
		}

		glEnd();

		glScalef(1.0f / 10.0f, 1.0f / 10.0f, 1.0f / 10.0f);
		glTranslatef(0.5f, 10.0f, 5.0f);

		// cube test end

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		for (unsigned int x = 0; x < m_nWidth; x++) {
			for (unsigned int z = 0; z < m_nHeight; z++) {
				glPushMatrix();

				// Translate the cube to the position defined by the grid(x,z) and the perlin noise (y)
				glTranslatef(-0.5f + (float)x - (float)m_nWidth / 2.0f, -2.5f - (float)m_pData[z * m_nHeight + x], -0.5f - float(z) + (float)m_nHeight / 2.0f);

				// This function will draw the complete buffer
				glDrawElements(GL_TRIANGLES, INDICES, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
				glPopMatrix();
			}
		}

		glPopMatrix();
		glFlush();
		glTranslatef(-0.5f, -10.0f, 5.0f);

		angle++;
		//if (angle > 360) angle = 0;

		window.Present();
	}

	return 0;
}
