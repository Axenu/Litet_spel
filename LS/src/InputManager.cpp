#include "InputManager.h"

EventManager* EventManager::_Instance;

// GLFW key callbacks.
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE)
    {
         glfwSetWindowShouldClose(win, 1);
    }
    else
    {
        EventManager *myEventManager = EventManager::Instance();
        myEventManager->execute("key", key, action);
    }
}

void mouse_key_callback(GLFWwindow* win, int button, int action, int mods) {
    EventManager *myEventManager = EventManager::Instance();
    myEventManager->execute("button", button, action);
}

void cursorPosition_callback(GLFWwindow* win, double x, double y) {
    EventManager *myEventManager = EventManager::Instance();
    myEventManager->execute("mouse", x, y);
}

InputManager::InputManager(GLFWwindow *window)
{
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_key_callback);
    glfwSetCursorPosCallback(window, cursorPosition_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
InputManager::~InputManager()
{

}
