#include "Zombie.h"
#include "Human.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Bengine/include/ResourceManager.h>

Zombie::Zombie()
{
    //ctor
}

Zombie::~Zombie()
{
    //dtor
}

void Zombie::init(float speed, const glm::vec2& pos, const glm::vec2& dir){
    m_speed = speed;
    m_position = pos;
    m_direction = dir;
    m_health = ZOMBIE_HEALTH;
    //m_color = Bengine::ColorRGBA8(0, 180, 0, 255);
    m_color = Bengine::ColorRGBA8(ZOMBIE_COLOR_R, ZOMBIE_COLOR_G, ZOMBIE_COLOR_B, ZOMBIE_COLOR_A);
    m_textureID = Bengine::ResourceManager::getTexture("textures/zombie.png").id;
}

void Zombie::init(float speed, const glm::vec2& pos){
    m_speed = speed;
    m_position = pos;
    m_health = ZOMBIE_HEALTH;
    //m_color = Bengine::ColorRGBA8(0, 180, 0, 255);
    m_color = Bengine::ColorRGBA8(ZOMBIE_COLOR_R, ZOMBIE_COLOR_G, ZOMBIE_COLOR_B, ZOMBIE_COLOR_A);
    m_textureID = Bengine::ResourceManager::getTexture("textures/zombie.png").id;
}

bool Zombie::receiveDamage(int damageVal){
    m_health -= damageVal;

    //Darken color according to health
    m_color.r = (GLubyte)(ZOMBIE_COLOR_R * (m_health / ZOMBIE_HEALTH));
    m_color.g = (GLubyte)(ZOMBIE_COLOR_G * (m_health / ZOMBIE_HEALTH));
    m_color.b = (GLubyte)(ZOMBIE_COLOR_B * (m_health / ZOMBIE_HEALTH));

    if(m_health <= 0){
        return true;
    }

    return false;
}

void Zombie::update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies,
                            float deltaTime){

    //Move Zombie towards closest human
    Human* nearestHuman = getNearestHuman(humans);
    if(nearestHuman != nullptr){
        ///Rotation will take place gradually.
        glm::vec2 prevDir = glm::normalize(m_direction);
        glm::vec2 objDir = glm::normalize(nearestHuman->getPosition() - m_position);
        //float angle = acos(glm::dot(prevDir, objDir));
        float angle = glm::orientedAngle(prevDir, objDir);
//
        if(angle > ZOMBIE_TURN_SPEED) angle = ZOMBIE_TURN_SPEED;
        if(angle < -ZOMBIE_TURN_SPEED) angle = -ZOMBIE_TURN_SPEED;

        m_direction = glm::rotate(prevDir, angle * deltaTime);///< Rotation is also dependant on @param "deltaTime"

        //m_direction = glm::normalize(nearestHuman->getPosition() - m_position);
        m_position += m_direction * m_speed * deltaTime;
    }

    collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans){
    Human* closestHuman = nullptr;
    float smallestDistance = 0;
    bool firstHuman = true;

    for(auto it = humans.begin(); it != humans.end(); it++){

        glm::vec2 distVec = (*it)->getPosition() - m_position;
        float dist = glm::length(distVec);

        if( firstHuman == true ){

            closestHuman = *it;
            smallestDistance = dist;
            firstHuman = false;

        }else if( dist < smallestDistance ){

            closestHuman = *it;
            smallestDistance = dist;

        }
    }
    return closestHuman;

}
