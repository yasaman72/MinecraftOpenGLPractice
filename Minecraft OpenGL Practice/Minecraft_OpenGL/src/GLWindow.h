// --------------------------------------------------------------------------------------
//	OpenGL Triangle 
//	GLWindow.h
// --------------------------------------------------------------------------------------
#pragma once

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <glfw3.h>

// --------------------------------------------------------------------------------

class GLWindow
{
public:
	int						m_nWidth;
	int						m_nHeight;
	std::string		m_strTitle;
	GLFWwindow* m_Window;

public:
	float					m_fAspect;

public:
	GLWindow(int nWidth, int nHeight, std::string strTitle);
	~GLWindow();

	bool Init(void);

	static void ErrorCallback(int error, const char* description);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool IsRunning(void);
	void Present(void);

};
