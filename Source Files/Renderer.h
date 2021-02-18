#pragma once

#include <GL/glew.h>
#include "Shader.h"
#include "VertexArray.h"

// DEBUGGING TOOL

// Comment this in or out for Debugging
#define DEBUG

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(__FILE__,__LINE__))
#else
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* file, int line);


class Renderer
{
public:
	void ClearScreen() const;
	void Draw(const VertexArray& va, const Shader& shader) const;
};