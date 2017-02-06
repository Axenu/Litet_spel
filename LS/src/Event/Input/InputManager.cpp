#include "Event/Input/InputManager.h"


// GLFW key callbacks.
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
         glfwSetWindowShouldClose(win, 1);
    }
    else
    {
        InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        KeyboardEvent event(key, action);
        iManager->getManager()->execute(event);
    }
}

void mouse_key_callback(GLFWwindow* win, int key, int action, int mods)
{
    InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    MouseClickEvent event(key, action);
    iManager->getManager()->execute(event);
}

void cursorPosition_callback(GLFWwindow* win, double x, double y)
{
    InputManager* iManager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
    MouseMoveEvent event(x, y);
    iManager->getManager()->execute(event);
}

void InputManager::switchCursorMode(const cursorModeChangeEvent& event)
{
    glfwSetInputMode(_window, GLFW_CURSOR, event.getState());
}

void InputManager::quitGame(const QuitGameEvent& event)
{
    glfwSetWindowShouldClose(_window, 1);
}

InputManager::InputManager(GLFWwindow *window, EventManager* manager) : _window(window), _manager(manager)
{
    _cursorMode = GLFW_CURSOR_NORMAL;
    glfwSetWindowUserPointer(_window, this);
    glfwSetKeyCallback(_window, key_callback);
    glfwSetMouseButtonCallback(_window, mouse_key_callback);
    glfwSetCursorPosCallback(_window, cursorPosition_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, _cursorMode);
    _manager->listen(this, &InputManager::switchCursorMode);
    _manager->listen(this, &InputManager::quitGame);
}
InputManager::~InputManager()
{

}

EventManager* InputManager::getManager()
{
    return _manager;
}
