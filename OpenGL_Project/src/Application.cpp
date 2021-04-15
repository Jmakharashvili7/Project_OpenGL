#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "structures.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
        Renderer renderer;

        float positions[] = {
           -0.5f, -0.5f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, 1.0f, 0.0f, // 1
            0.5f,  0.5f, 1.0f, 1.0f, // 2
           -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

       unsigned int indices[] = {
           0, 1, 2,
           2, 3, 0
       };

       VertexArray va;
       VertexBuffer vb(positions, 4 * 4 * sizeof(float));

       VertexBufferLayout layout;
       layout.Push<float>(2);
       layout.Push<float>(2);
       va.AddBuffer(vb, layout);

       IndexBuffer ib(indices, 6);

       // set the projection matrix to orgthographic matrix 
       glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

       Shader shader("res/shaders/Basic.Shader");
       shader.Bind();
       
       shader.setVec4("u_color", 0.2f, 0.3f, 0.8f, 1.0f);

       // setup the texture and bind it
       Texture2D texture("res/textures/test.jpg");
       texture.Bind();
       // pass in uniform for texture (value must be matching the location of slot texture is bound to)
       shader.setInt("u_texture", 0);

       /* Loop until the user closes the window */
       while (!glfwWindowShouldClose(window))
       {
           /* Render here */
           renderer.Clear();
            
           renderer.Draw(va, ib, shader);

           /* Swap front and back buffers */
           glfwSwapBuffers(window);

           /* Poll for and process events */
           glfwPollEvents();
       }
    }

    glfwTerminate();
    return 0;
}