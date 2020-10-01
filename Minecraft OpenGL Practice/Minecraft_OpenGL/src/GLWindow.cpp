// --------------------------------------------------------------------------------------
//	OpenGL Triangle 
//	GLWindow.cpp
// --------------------------------------------------------------------------------------
#include "GLWindow.h"


using namespace std;

// --------------------------------------------------------------------------------

GLWindow::GLWindow(int nWidth, int nHeight, string strTitle)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_strTitle = strTitle;

	m_fAspect = (float)m_nWidth / (float)m_nHeight;
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool GLWindow::Init(void)
{
	glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit()) return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	m_Window = glfwCreateWindow(m_nWidth, m_nHeight, m_strTitle.c_str(), NULL, NULL);
	if (!m_Window) {
		glfwTerminate();
		return false;
	}

	glfwGetFramebufferSize(m_Window, &m_nWidth, &m_nHeight);
	glfwSetKeyCallback(m_Window, KeyCallback);

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);

	glewInit();

	return true;
}

void GLWindow::ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void GLWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

bool GLWindow::IsRunning(void)
{
	if (glfwWindowShouldClose(m_Window)) return false;
	return true;
}

void GLWindow::Present(void)
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}