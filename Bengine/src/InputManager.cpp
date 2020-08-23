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

void InputManager::update(){

    //Copy keymap into prevKeyMap
    for(auto& it : _keyMap){
        _prevKeyMap[it.first] = it.second;
    }
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

bool InputManager::isKeyDown(unsigned int keyID) const{
    //We dont want to create a key if it doesn't exist
    auto it = _keyMap.find(keyID);
    if(it != _keyMap.end()){
        //Found the key
        return it->second;
    }
    //Didn't find the key
    return false;
}

bool InputManager::isKeyPressed(unsigned int keyID) const{
    //Check if it has been pressed this frame
    if( isKeyDown(keyID) && (!(wasKeyDown(keyID))) ){
        return true;
    }
    return false;
}

bool InputManager::wasKeyDown(unsigned int keyID) const{

    //We dont want to create a key if it doesn't exist
    auto it = _prevKeyMap.find(keyID);
    if(it != _prevKeyMap.end()){
        //Found the key
        return it->second;
    }
    //Didn't find the key
    return false;
}

}
