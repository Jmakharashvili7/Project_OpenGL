#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "StaticObjects.h"
#include "structures.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl; 
        return false;
    }
    return true;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
    {
        std::cerr << "GLFW could not be initialized!" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize the GLEW library
       MUST BE before the rendering context*/
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW could not initialized!" << std::endl;
        return -2;
    } 

    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // setup and bind the vertex buffer
    unsigned int vertex_buffer;
    GLCall(glGenBuffers(1, &vertex_buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    // setup the layer of the buffer
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    // setup and bind the vertex buffer
    unsigned int indices_buffer;
    GLCall(glGenBuffers(1, &indices_buffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // setup the shader
    ShaderProgramSource shader_src = ParseShader("res/shaders/Basic.Shader");

    unsigned int shader = CreateShader(shader_src.VertexSource, shader_src.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_1PASS_EXT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // delete the shader program
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}