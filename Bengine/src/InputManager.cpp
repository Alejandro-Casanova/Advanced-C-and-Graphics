#include "InputManager.h"

namespace Bengine{

InputManager::InputManager() :
    _mouseCoords(0.0f)
{
    //ctor
}

InputManager::~InputManager()
{
    //dtor
}

void InputManager::pressKey(unsigned int keyID){
    _keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID){
    _keyMap[keyID] = false;
}

void InputManager::setMouseCoords(float x, float y){
    _mouseCoords.x = x;
    _mouseCoords.y = y;
}

bool InputManager::isKeyPressed(unsigned int keyID) const{
    auto it = _keyMap.find(keyID);
    if(it != _keyMap.end()){
        return it->second;
    }
    return false;
}

}
