#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

// Debugging macros
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// macro for easy logging (will only log during debug mode)
#ifdef _DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x) 
#endif // LOG()

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
};
#endif // RENDERER_H

