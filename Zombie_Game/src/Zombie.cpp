#include "Zombie.h"
#include "Human.h"

Zombie::Zombie()
{
    //ctor
}

Zombie::~Zombie()
{
    //dtor
}

void Zombie::init(float speed, glm::vec2 pos){
    _speed = speed;
    _position = pos;
    _health = 150.0f;
    _color = Bengine::ColorRGBA8(0, 180, 0, 255);
}

void Zombie::update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies){

    //Move Zombie towards closest human
    Human* nearestHuman = getNearestHuman(humans);
    if(nearestHuman != nullptr){
        glm::vec2 dirVec = (nearestHuman->getPosition() - _position);
        _position += glm::normalize(dirVec) * _speed;
    }

    collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans){
    Human* closestHuman = nullptr;
    float smallestDistance = 0;
    bool firstHuman = true;

    for(auto it = humans.begin(); it != humans.end(); it++){

        glm::vec2 distVec = (*it)->getPosition() - _position;
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
