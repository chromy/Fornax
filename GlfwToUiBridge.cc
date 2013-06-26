#include <GL/glfw.h>
#include "GlfwToUiBridge.hh"

void GLFWCALL window_resize(int width, int height);
void GLFWCALL mouse_button_event(int button, int action);
void GLFWCALL mouse_pos_event(int x, int y);
void GLFWCALL mouse_wheel_event(int pos);

static UserInterface::View* view = NULL;

void glfw_to_ui_bridge(UserInterface::View* new_view) {
    view = new_view;
    glfwSetWindowSizeCallback(window_resize);
    glfwSetMouseButtonCallback(mouse_button_event);
    glfwSetMousePosCallback(mouse_pos_event);
    glfwSetMouseWheelCallback(mouse_wheel_event);
}

void GLFWCALL window_resize(int width, int height) {
    view->resize(width, height);
}

void GLFWCALL mouse_button_event(int button, int action) {
    if (action == GLFW_PRESS) {
        view->mouse_button_down(button);
    } else if (action == GLFW_RELEASE) {
        view->mouse_button_up(button);
    }
}

void GLFWCALL mouse_pos_event(int x, int y) {
    view->mouse_move(x, y);
}

void GLFWCALL mouse_wheel_event(int pos) {
    //TODO: fix
    view->mouse_wheel(pos);
}
