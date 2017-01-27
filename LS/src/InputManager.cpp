#include "InputManager.h"

void InputManager::subscribeToKey(int keyID, keyCallback cb)
{
    if (keyID > 399)
    {
        //error
        return;
    }
    _keyCallbackList[keyID].push_back(cb);
}
void InputManager::subscribeToMouseMove(mouseMoveCallback cb)
{
    _mouseMoveCallbackList.push_back(cb);
}
void InputManager::subscribeToMouseClick(int buttonID, mouseClickCallback cb)
{
    if (buttonID > 7)
    {
        //error
        return;
    }
    _mouseClickCallbackList[buttonID].push_back(cb);
}
void InputManager::fireKey(int keyID, int param)
{
    if (keyID > 399)
    {
        //error
        return;
    }
    for(keyCallback& cb : _keyCallbackList[keyID])
    {
        cb(keyID, param);
    }
}
void InputManager::fireMouseMove(double x, double y)
{
    for(mouseMoveCallback& cb : _mouseMoveCallbackList)
    {
        cb(x, y);
    }
}
void InputManager::fireMouseClick(int buttonID, int state)
{
    if (buttonID > 7)
    {
        //error
        return;
    }
    for(mouseClickCallback& cb : _mouseClickCallbackList[buttonID])
    {
        cb(buttonID, state);
    }
}
InputManager::InputManager()
{
    _keyCallbackList = std::vector<std::vector<keyCallback>>(400);
    _mouseClickCallbackList = std::vector<std::vector<mouseClickCallback>>(8);
}
InputManager::~InputManager()
{

}
