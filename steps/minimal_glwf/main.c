#include <stdio.h>
#include <GL/glfw.h>

int main(void) {
    int running = GL_TRUE;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    if (!glfwOpenWindow(640, 480, 8, 8, 8, 0, 24, 0, GLFW_WINDOW))
        return -1;

    /* Loop until the user closes the window */
    while (running) {
        /* Render here */
        glClear( GL_COLOR_BUFFER_BIT );

        /* Swap front and back buffers and process events */
        glfwSwapBuffers();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
                  glfwGetWindowParam( GLFW_OPENED );
    }

    return 0;
}

