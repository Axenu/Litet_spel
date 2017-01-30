#include "InputManager.h"


// GLFW key callbacks.
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE)
    {
         glfwSetWindowShouldClose(win, 1);
    } else if (key == GLFW_KEY_T)
    {
        if (action == GLFW_PRESS)
        {
            InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
            iManager->switchCursorMode(win);
        }
    }
    else
    {
        InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        iManager->getManager()->handleEvent(new KeyboardEvent(key, action));
    }
}

void mouse_key_callback(GLFWwindow* win, int key, int action, int mods) {
    InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    iManager->getManager()->handleEvent(new MouseClickEvent(key, action));
}

void cursorPosition_callback(GLFWwindow* win, double x, double y) {
    InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    iManager->getManager()->handleEvent(new MouseMoveEvent(x, y));
}

void InputManager::switchCursorMode(GLFWwindow *window)
{
    if (_cursorMode == GLFW_CURSOR_NORMAL)
    {
        _cursorMode = GLFW_CURSOR_DISABLED;
    }
    else
    {
        _cursorMode = GLFW_CURSOR_NORMAL;
    }
    glfwSetInputMode(window, GLFW_CURSOR, _cursorMode);
}

InputManager::InputManager(GLFWwindow *window, EventManager* manager) : _manager(manager)
{
    _cursorMode = GLFW_CURSOR_NORMAL;
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_key_callback);
    glfwSetCursorPosCallback(window, cursorPosition_callback);
    glfwSetInputMode(window, GLFW_CURSOR, _cursorMode);
}
InputManager::~InputManager()
{

}

EventManager* InputManager::getManager()
{
    return _manager;
}
