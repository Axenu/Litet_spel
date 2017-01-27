#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <map>

typedef std::function<void(int keyID, int param)> keyCallback;
typedef std::function<void(double x, double y)> mouseMoveCallback;
typedef std::function<void(int buttonID, int action)> mouseClickCallback;

class InputManager
{
public:
    void subscribeToKey(int keyID, keyCallback cb);
    void subscribeToMouseMove(mouseMoveCallback cb);
    void subscribeToMouseClick(int buttonID, mouseClickCallback cb);
    void fireKey(int keyID, int param);
    void fireMouseMove(double x, double y);
    void fireMouseClick(int buttonID, int state);
    InputManager();
    ~InputManager();
private:
    std::vector<std::vector<keyCallback>> _keyCallbackList;
    std::vector<mouseMoveCallback> _mouseMoveCallbackList;
    std::vector<std::vector<mouseClickCallback>> _mouseClickCallbackList;
};
