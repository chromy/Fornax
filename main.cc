#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#include <GL/glfw.h>

// Various included headers
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

#include "UserInterface.hh"
#include "GlfwToUiBridge.hh"

// Various macro definitions
#define WIDTH   800
#define HEIGHT  800

static UserInterface::View* view;

void ready2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glDisable(GL_DEPTH_TEST);
}

double last_time = 0;
int num_frames = 0;
double ms_for_frame = 1;
int frame_cost() {
    // Measure speed
    num_frames++;
    double current_time = glfwGetTime();
    if (current_time - last_time >= 1.0) { // If last prinf() was more than 1 sec ago
         ms_for_frame = 1000.0/((double)num_frames);
         printf("%fms/frame (%dfps)\n", ms_for_frame, num_frames);
         num_frames = 0;
         last_time += 1.0;
         //((BitmapSurface*)view->surface())->SaveToJPEG(WSLit("./result.jpg"));
    }
    return ms_for_frame;
}

void ready3D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(45, (float)width/height, 0.1, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
}

void drawUI(void) {
    int width = view->width();
    int height = view->height();
    GLuint texture = view->get_texture();

    ready2D(width, height);

    glBindTexture(GL_TEXTURE_2D, texture);  //Retrieved from your object
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glColor4f(1.0,  1.0,  1.0, 1.0);
        glTexCoord2f(0, 0); glVertex2f(0,0);
        glTexCoord2f(1.0f, 0); glVertex2f(width,0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
        glTexCoord2f(0, 1.0f); glVertex2f(0, height);
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

static  int x = 0;
static  int y = 0;
void drawScene(void) {
    int width = view->width();
    int height = view->height();
    ready3D(width, height);
    // Reset transformations
    glLoadIdentity();
    gluLookAt(10, 10, 10, 0, 0, 0, 1, 0, 0);

    // Rotate when user changes rotate_x and rotate_y
    glRotatef(x++, 1.0, 0.0, 0.0 );
    glRotatef(y++, 0.0, 1.0, 0.0 );
    //glTranslatef(2.0, 0.0, +100.0);

    // Gray side - BACK
    glBegin(GL_QUADS);
    glColor3f(   0.5,  0.5, 0.5 );
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();

    // Purple side - RIGHT
    glBegin(GL_QUADS);
    glColor3f(  1.0,  0.0,  1.0 );
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();

    // Green side - LEFT
    glBegin(GL_QUADS);
    glColor3f(   0.0,  1.0,  0.0 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    // Blue side - TOP
    glBegin(GL_QUADS);
    glColor3f(   0.0,  0.0,  1.0 );
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();

    // Red side - BOTTOM
    glBegin(GL_QUADS);
    glColor3f(   1.0,  0.0,  0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
}

bool loop(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    drawUI();
    glfwSwapBuffers();
    UserInterface::update();
    frame_cost();

    // Check if ESC key was pressed or window was closed
    return !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
}

int main(void) {
    int running = GL_TRUE;

    // Initialize the GLFW
    if (!glfwInit())
        return -1;

    // Create a window
    if (!glfwOpenWindow(WIDTH, HEIGHT, 8, 8, 8, 0, 24, 0, GLFW_WINDOW))
        return -1;

    UserInterface::initialise();

    view = new UserInterface::View(WIDTH, HEIGHT);

    // Connect events to ui.
    glfw_to_ui_bridge(view);

    // Main loop.
    while (running) {
        running = loop();
    }

    UserInterface::finalise();

    return 0;
}
