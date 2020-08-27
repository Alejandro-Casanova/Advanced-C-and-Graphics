#include "Human.h"
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <Bengine/include/ResourceManager.h>

Human::Human() {}

Human::~Human()
{
    //dtor
}

void Human::init(float speed, glm::vec2 pos){

    static std::mt19937 eng{std::random_device{}()}; //Create random engine object
    static std::uniform_real_distribution<float> realDist(-1.0f, 1.0f); //Create uniform real distribution object

    m_speed = speed;
    m_health = 20.0f;
    m_position = pos;
    m_color = Bengine::ColorRGBA8(255, 255, 255, 255);
    m_direction = glm::vec2(realDist(eng), realDist(eng));

    //Make sure direction is not set to 0
    if(m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

    m_direction = glm::normalize(m_direction);

    m_textureID = Bengine::ResourceManager::getTexture("textures/human.png").id;


}

void Human::update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies,
                            float deltaTime){

    static std::mt19937 eng{std::random_device{}()}; //Create random engine object
    //static std::uniform_real_distribution<float> roamDist(glm::radians(-1.0f), glm::radians(1.0f));
    static std::uniform_real_distribution<float> roamDist(glm::radians(-135.0f), glm::radians(135.0f)); //Create uniform real distribution object
    static std::uniform_real_distribution<float> collideDist(glm::radians(135.0f), glm::radians(270.0f));

    static int frames = 0;



    ///If collide with wall, change direction
    if(collideWithLevel(levelData)){

        float wallAngle = collideDist(eng);
        ///Ensure shortest turn away from wall
        if(wallAngle > glm::radians(180.0f)){
            wallAngle -= (2 * M_PI);
        }
        m_directionObjective = glm::rotate(m_direction, wallAngle);

    }
    ///Randomly change direction
    else if(frames++ == 150){

        m_directionObjective = glm::rotate(m_direction, roamDist(eng));
        frames = 0;

    }

    float angle = glm::orientedAngle(m_direction, m_directionObjective);

//    if(angle > HUMAN_TURN_SPEED) angle = HUMAN_TURN_SPEED;
//    if(angle < -HUMAN_TURN_SPEED) angle = -HUMAN_TURN_SPEED;

    m_direction = glm::rotate(m_direction, angle / 20.0f * deltaTime);

    m_position += m_direction * m_speed * deltaTime;
}

template<class T>
int sign(T val){
    return ( ( T(0) < val ) - ( val < T(0) ) );
}

bool Human::receiveInfection(int infectVal){
    m_infectionResistance -= infectVal;

    m_color.r -= infectVal * (255 - ZOMBIE_COLOR_R) / HUMAN_INFECT_RESIST;
    //m_color.g += infectVal * 180 / HUMAN_INFECT_RESIST;
    m_color.g -= infectVal * (255 - ZOMBIE_COLOR_G) / HUMAN_INFECT_RESIST;
    m_color.b -= infectVal * (255 - ZOMBIE_COLOR_B) / HUMAN_INFECT_RESIST;


    if(m_infectionResistance <= 0) return true;

    return false;

}
