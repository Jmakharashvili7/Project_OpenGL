#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

class VertexArray;
class IndexBuffer;
class Shader;

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
public:
    void Clear();
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
#endif // RENDERER_H

