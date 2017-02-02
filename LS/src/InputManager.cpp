#include "InputManager.h"


// GLFW key callbacks.
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE)
    {
         glfwSetWindowShouldClose(win, 1);
    }
    else
    {
        InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
		KeyboardEvent e(key, action);
        iManager->getManager()->execute(&e);
    }
}

void mouse_key_callback(GLFWwindow* win, int key, int action, int mods) {
    InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
	MouseClickEvent e(key, action);
    iManager->getManager()->execute(&e);
}

void cursorPosition_callback(GLFWwindow* win, double x, double y) {
    InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
	MouseMoveEvent e(x, y);
    iManager->getManager()->execute(&e);
}

void InputManager::switchCursorMode(const cursorModeChangeEvent* event)
{
    glfwSetInputMode(_window, GLFW_CURSOR, event->getState());
}
// int InputManager::getCursorMode()
// {
//     return _cursorMode;
// }

InputManager::InputManager(GLFWwindow *window, EventManager* manager) : _window(window), _manager(manager)
{
    // _cursorMode = GLFW_CURSOR_NORMAL;
    glfwSetWindowUserPointer(_window, this);
    glfwSetKeyCallback(_window, key_callback);
    glfwSetMouseButtonCallback(_window, mouse_key_callback);
    glfwSetCursorPosCallback(_window, cursorPosition_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    _manager->listen(this, &InputManager::switchCursorMode);
}
InputManager::~InputManager()
{

}

EventManager* InputManager::getManager()
{
    return _manager;
}
