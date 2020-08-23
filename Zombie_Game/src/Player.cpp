#include "Player.h"

#include "Gun.h"

#include <SDL2/SDL.h>

Player::Player() :
    _inputManager(nullptr),
    _currentGun(-1),
    _camera(nullptr),
    _bullets(nullptr){

}

Player::~Player()
{
    for(auto it = _guns.begin(); it != _guns.end(); it++){
        delete (*it);
    }
}

void Player::init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager,
                                            Bengine::Camera2D* camera,
                                            std::vector<Bullet>* bullets){

    _speed = speed;
    _position = pos;
    _color = Bengine::ColorRGBA8(0, 0, 185, 255);
    _inputManager = inputManager;
    _camera = camera;
    _bullets = bullets;
    _health = 150.0f;
}

void Player::addGun(Gun* gun){
    _guns.push_back(gun);

    //If no guns in inventory, equip gun
    if(_currentGun == -1){
        _currentGun = 0;
    }

}

void Player::update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies){

    if(_inputManager->isKeyDown(SDLK_w)){
        _position.y += _speed;
    }
    if(_inputManager->isKeyDown(SDLK_s)){
        _position.y -= _speed;
    }
    if(_inputManager->isKeyDown(SDLK_a)){
        _position.x -= _speed;
    }
    if(_inputManager->isKeyDown(SDLK_d)){
        _position.x += _speed;
    }

    //Swap weapons
    if( ( _inputManager->isKeyDown(SDLK_1) ) && ( _guns.size() >= 0 ) ){
        _currentGun = 0;
    }
    if( ( _inputManager->isKeyDown(SDLK_2) ) && ( _guns.size() >= 1 ) ){
        _currentGun = 1;
    }
    if( ( _inputManager->isKeyDown(SDLK_3) ) && ( _guns.size() >= 2 ) ){
        _currentGun = 2;
    }

    //Shoot
    if(_currentGun != -1){

        glm::vec2 mousePos = _camera->convertScreenToWorld(_inputManager->getMouseCoords());
        glm::vec2 centerPlayerPosition = _position + AGENT_RADIUS;
        glm::vec2 direction = glm::normalize(mousePos - centerPlayerPosition);

        _guns[_currentGun]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
                                   centerPlayerPosition,
                                   direction,
                                   *_bullets);
    }



    collideWithLevel(levelData);

}
