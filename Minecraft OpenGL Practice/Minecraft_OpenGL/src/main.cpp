#include <math.h>

#include "GLWindow.h"
#include "Bitmap.h"
#include "GL/glew.h"
#include "PerlinNoise.h"
#include "PerlinNoise.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"

#include <iostream>

using namespace std;

// --------------------------------------------------------------------------------

#define VERTICES		24
#define INDICES			36
#define CLOUDAMOUNT		200

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

float cloudXPosMultiplier = 0.0f;

float	vert[] = { -0.5,-0.5,0.5, 0.5,-0.5,0.5f, 0.5,0.5,0.5, -0.5,0.5,0.5, -0.5,-0.5,-0.5, 0.5,-0.5,-0.5f, 0.5,0.5,-0.5, -0.5,0.5,-0.5 };
float	uv[] = { 0.25,0.0, 0.5,0.0, 0.5,0.33, 0.25,0.33, 0.75,0.33, 0.75,0.66, 0.5,0.66, 0.5,0.33, 0.5,1.0, 0.25,1.0, 0.25,0.66, 0.5,0.66, 0.0,0.66, 0.0,0.33, 0.25,0.33, 0.25,0.66, 0.25,0.33, 0.5,0.33, 0.5,0.66, 0.25,0.66, 1.0,0.33, 1.0,0.66, 0.75,0.66, 0.75,0.33 };
int		index[] = { 0,1,2,3, 1,5,6,2, 5,4,7,6, 4,0,3,7, 3,2,6,7, 1,0,4,5 };
float	uvsky[] = { 0.0,0.33, 0.25,0.33, 0.25,0.66, 0.0,0.66, 0.25,0.33, 0.5,0.33, 0.5,0.66, 0.25,0.66, 0.5,0.33, 0.75,0.33, 0.75,0.66, 0.5,0.66, 0.75,0.33, 1.0,0.33, 1.0,0.66, 0.75,0.66, 0.25,1.0, 0.25,0.66, 0.5,0.66, 0.5,1.0, 0.25,0.0, 0.5,0.0, 0.5,0.33, 0.25,0.33 };
float cloudStartPositions[CLOUDAMOUNT][3];
float cloudScales[CLOUDAMOUNT][2];


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 0.5f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void DrawGound(GLuint& cubeTexture, unsigned int& width, unsigned int& height, unsigned char* pData)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int z = 0; z < height; z++) {
			glPushMatrix();

			// Translate the cube to the position defined by the grid(x,z) and the perlin noise (y)
			glTranslatef(-0.5f + (float)x - (float)width / 2.0f, -2.5f - (float)pData[z * height + x], -0.5f - float(z) + (float)height / 2.0f);

			// This function will draw the complete buffer
			glDrawElements(GL_TRIANGLES, INDICES, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
			glPopMatrix();
		}
	}

	glPopMatrix();
}

void GenerateCloudStartPosition()
{
	for (size_t i = 0; i < CLOUDAMOUNT; i++)
	{
		cloudStartPositions[i][0] = -300 + (rand() % 600);
		cloudStartPositions[i][1] = 10 + (rand() % 50);
		cloudStartPositions[i][2] = -200 + (rand() % 300);

	}
}

void GenerateCloudScale()
{
	for (size_t i = 0; i < CLOUDAMOUNT; i++)
	{
		cloudScales[i][0] = 10 + (rand() % 20);
		cloudScales[i][1] = 5 + (rand() % 25);
	}
}

void DrawCloud(GLuint& cloudTexture, float cloudStartPosition[3], float cloudScale[2])
{
	glPushMatrix();
	glTranslatef(cloudStartPosition[0] + cloudXPosMultiplier, cloudStartPosition[1], cloudStartPosition[2]);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glScalef(cloudScale[0], 2.0f, cloudScale[1]);
	glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, cloudTexture);

	glBegin(GL_QUADS);
	for (int i = 0; i < 24; i++) {
		glTexCoord2f(uvsky[i * 2], uvsky[i * 2 + 1]);
		glVertex3f(vert[index[i] * 3], vert[index[i] * 3 + 1], vert[index[i] * 3 + 2]);
	}

	glEnd();

	glPopMatrix();
}

void DrawSkyBox(GLuint& skyTexture)
{
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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
	glPopMatrix();
}




// --------------------------------------------------------------------------------

int main(void)
{
	GLWindow window(1660, 900, "Minecraft Project");
	if (window.Init() == false) exit(EXIT_FAILURE);

	glfwSetCursorPosCallback(window.m_Window, Mouse_Callback);


	GLuint	skyTexture;
	GLuint	cloudTexture;

	GLuint	cubeTexture;
	GLfloat aspect = (GLfloat)1280 / 720;

	GLfloat angle = 0.0f;

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glShadeModel(GL_FLAT);

	// The Texture

	CBitmap* pBitmap = new CBitmap();
	pBitmap->LoadFromFile("Bitmap/dirt.jpg");

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

	unsigned int m_nWidth = 200;
	unsigned int m_nHeight = 200;

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



	// could
	CBitmap* pCloud = new CBitmap();
	pCloud->LoadFromFile("Bitmap/could.jpg");

	glGenTextures(1, &cloudTexture);
	glBindTexture(GL_TEXTURE_2D, cloudTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pCloud->m_nWidth, pCloud->m_nHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pCloud->m_pBytes);
	delete(pCloud);



	// Fill the grid with values from the noise function

	for (unsigned int i = 0; i < m_nHeight; ++i) {     // y
		for (unsigned int j = 0; j < m_nWidth; ++j) {  // x
			double x = (double)j / ((double)m_nWidth);
			double y = (double)i / ((double)m_nHeight);

			double n = pn.noise(5 * x, 5 * y, 0.8) * pn.noise(10 * x, 10 * y, 0.2);

			m_pData[inc] = (unsigned char)floor(30 * n);

			inc++;
		}
	}

	GenerateCloudStartPosition();
	GenerateCloudScale();


	while (window.IsRunning()) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, aspect, 3.0, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0f, 0.8f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		
		processInput(window.m_Window);

		//gluLookAt(-cameraX, 0.0f, -cameraZ, px, py, pz, 0.0f, 1.0f, 0.0f);
		gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, (cameraFront + cameraPos).x, (cameraFront + cameraPos).y, (cameraFront + cameraPos).z, cameraUp.x, cameraUp.y, cameraUp.z);


		// skybox
		DrawSkyBox(skyTexture);

		// ground
		DrawGound(cubeTexture, m_nWidth, m_nHeight, m_pData);


		// rendering transparent objects ======
		// cloud
		for (int x = 0; x < CLOUDAMOUNT; x++)
		{
			DrawCloud(cloudTexture, cloudStartPositions[x], cloudScales[x]);
		}

		glPopMatrix();
		glFlush();
		glTranslatef(-0.5f, -10.0f, 5.0f);

		angle++;
		cloudXPosMultiplier += 0.05f;
		//if (angle > 360) angle = 0;

		Sleep(30);
		window.Present();
	}

	return 0;
}
