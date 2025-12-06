#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "initialize.h"

void processInput(GLFWwindow *window);

int main()
{
    OpenGLVersion version;
    version.major = 3;
    version.minor = 3;

    OpenGLWindowSize windowSize;
    windowSize.width = 800;
    windowSize.height = 600;

    GLFWwindow* window = initialize(
        version,
        windowSize
    );

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
