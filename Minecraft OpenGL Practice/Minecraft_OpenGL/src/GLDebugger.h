#pragma once
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogError(#x, __FILE__, __LINE__))\

static void GLClearError()
{
	while (glGetError() != GL_ZERO);
}

static bool GLLogError(const char* function, const char* file, unsigned int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error  << "): "<< function 
			<< file << line << std::endl;
		return false;
	}

	return true;
}