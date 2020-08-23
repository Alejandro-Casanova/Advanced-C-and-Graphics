#include "Human.h"
#include <random>
#include <glm/gtx/rotate_vector.hpp>

Human::Human() : _direction(0.0f)
{
    //ctor
}

Human::~Human()
{
    //dtor
}

void Human::init(float speed, glm::vec2 pos){

    static std::mt19937 eng{std::random_device{}()}; //Create random engine object
    static std::uniform_real_distribution<float> realDist(-1.0f, 1.0f); //Create uniform real distribution object

    _speed = speed;
    _health = 20.0f;
    _position = pos;
    _color = Bengine::ColorRGBA8(200, 0, 200, 255);
    _direction = glm::vec2(realDist(eng), realDist(eng));

    //Make sure direction is not set to 0
    if(_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

    _direction = glm::normalize(_direction);


}

void Human::update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies){

    static std::mt19937 eng{std::random_device{}()}; //Create random engine object
    static std::uniform_real_distribution<float> roamDist(glm::radians(0.0f), glm::radians(360.0f)); //Create uniform real distribution object
    static std::uniform_real_distribution<float> collideDist(glm::radians(135.0f), glm::radians(270.0f));

    _position += _direction * _speed;

    static int frames = 0;

    //If collide with wall, change direction
    if(collideWithLevel(levelData)){
        _direction = glm::rotate(_direction, collideDist(eng));
    }

    //Randomly change darection
    else if(frames++ == 150){
        _direction = glm::rotate(_direction, roamDist(eng));
        frames = 0;
    }
}
