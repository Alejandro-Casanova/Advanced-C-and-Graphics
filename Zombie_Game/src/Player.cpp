#include "Player.h"

#include "Gun.h"
#include <Bengine/include/ResourceManager.h>

#include <SDL2/SDL.h>

Player::Player() :
    m_inputManager(nullptr),
    m_currentGun(-1),
    m_camera(nullptr),
    m_bullets(nullptr){

}

Player::~Player()
{
    for(auto it = m_guns.begin(); it != m_guns.end(); it++){
        delete (*it);
    }
}

void Player::init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager,
                                            Bengine::Camera2D* camera,
                                            std::vector<Bullet>* bullets){

    m_speed = speed;
    m_position = pos;
    m_color = Bengine::ColorRGBA8(255, 255, 255, 255);
    m_inputManager = inputManager;
    m_camera = camera;
    m_bullets = bullets;
    m_health = 150.0f;
    m_textureID = Bengine::ResourceManager::getTexture("textures/player.png").id;
}

void Player::addGun(Gun* gun){
    m_guns.push_back(gun);

    //If no guns in inventory, equip gun
    if(m_currentGun == -1){
        m_currentGun = 0;
    }

}

void Player::update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies,
                            float deltaTime){

    if(m_inputManager->isKeyDown(SDLK_w)){
        m_position.y += m_speed * deltaTime;
    }
    if(m_inputManager->isKeyDown(SDLK_s)){
        m_position.y -= m_speed * deltaTime;
    }
    if(m_inputManager->isKeyDown(SDLK_a)){
        m_position.x -= m_speed * deltaTime;
    }
    if(m_inputManager->isKeyDown(SDLK_d)){
        m_position.x += m_speed * deltaTime;
    }

    //Swap weapons
    if( ( m_inputManager->isKeyDown(SDLK_1) ) && ( m_guns.size() >= 0 ) ){
        m_currentGun = 0;
    }
    if( ( m_inputManager->isKeyDown(SDLK_2) ) && ( m_guns.size() >= 1 ) ){
        m_currentGun = 1;
    }
    if( ( m_inputManager->isKeyDown(SDLK_3) ) && ( m_guns.size() >= 2 ) ){
        m_currentGun = 2;
    }

    ///Get player direction
    glm::vec2 mousePos = m_camera->convertScreenToWorld(m_inputManager->getMouseCoords());
    glm::vec2 centerPlayerPosition = m_position + AGENT_RADIUS;
    m_direction = glm::normalize(mousePos - centerPlayerPosition);

    ///Shoot
    if(m_currentGun != -1){

        m_guns[m_currentGun]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
                                   centerPlayerPosition,
                                   m_direction,
                                   *m_bullets,
                                   deltaTime);
    }



    collideWithLevel(levelData);

}

bool Player::receiveInfection(int infectVal){

    m_infectionResistance -= infectVal;

    m_color.r -= infectVal * 255 / HUMAN_INFECT_RESIST;
    m_color.g -= infectVal * 75 / HUMAN_INFECT_RESIST;
    m_color.b -= infectVal * 255 / HUMAN_INFECT_RESIST;
    //m_color.b -= infectVal * 185 / HUMAN_INFECT_RESIST;

    if(m_infectionResistance <= 0) return true;

    return false;
}
