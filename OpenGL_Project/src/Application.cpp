#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "structures.h"

#include "Shader.h"
#include "StaticObjects.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

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

    // set the frame rate limit to vsync
    glfwSwapInterval(1);

    /* Initialize the GLEW library
       MUST BE before the rendering context*/
    if (glewInit() != GLEW_OK)
    {
        LOG("GLEW could not initialized!");
        return -2;
    }
    
    // print out the version of openGL
    LOG(glGetString(GL_VERSION));

    // scope
    {
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

       VertexArray va;
       VertexBuffer vb(positions, 4 * 2 * sizeof(float));

       VertexBufferLayout layout;
       layout.Push<float>(2);
       va.AddBuffer(vb, layout);

       IndexBuffer ib(indices, 6);

       Shader shader("res/shaders/Basic.Shader");
       shader.Bind();
       
       shader.SetUniform4f("u_color", Vector4(0.2f, 0.3f, 0.8f, 1.0f));

       /* Loop until the user closes the window */
       while (!glfwWindowShouldClose(window))
       {
           /* Render here */
           glClear(GL_COLOR_BUFFER_BIT);
            
           va.Bind();
           ib.Bind(); 

           GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

           /* Swap front and back buffers */
           glfwSwapBuffers(window);

           /* Poll for and process events */
           glfwPollEvents();
       }
    }

    glfwTerminate();
    return 0;
}