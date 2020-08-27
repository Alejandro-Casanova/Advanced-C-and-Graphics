#include "Gun.h"

#include <random>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot,
         float spread, float bulletDamage, float bulletSpeed,
         Bengine::SoundEffect fireEffect) :
                    m_name(name),
                    m_fireRate(fireRate),
                    m_bulletsPerShot(bulletsPerShot),
                    m_spread(spread),
                    m_bulletSpeed(bulletSpeed),
                    m_bulletDamage(bulletDamage),
                    m_fireEffect(fireEffect){



}

Gun::~Gun()
{
    //dtor
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime){
    m_frameCounter += deltaTime;
    if( ( m_frameCounter >= m_fireRate ) && isMouseDown){
        fire(direction, position, bullets);
        m_frameCounter = 0;
    }
}

void Gun::fire(glm::vec2 direction, const glm::vec2& position, std::vector<Bullet>& bullets){

    static std::mt19937 eng{std::random_device{}()}; //Create random engine object
    std::uniform_real_distribution<float> realDist(-m_spread, m_spread); //Create uniform real distribution object

    m_fireEffect.play();

    for(int i = 0; i < m_bulletsPerShot; i++){
        bullets.push_back(Bullet(position, glm::rotate(direction, realDist(eng)), m_bulletSpeed, m_bulletDamage));
    }
}
