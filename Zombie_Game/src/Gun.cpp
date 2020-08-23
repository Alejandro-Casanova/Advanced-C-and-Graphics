#include "Gun.h"

#include <random>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed) :
                    _name(name),
                    _fireRate(fireRate),
                    _bulletsPerShot(bulletsPerShot),
                    _spread(spread),
                    _bulletSpeed(bulletSpeed),
                    _bulletDamage(bulletDamage),
                    _frameCounter(0){



}

Gun::~Gun()
{
    //dtor
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets){
    _frameCounter++;
    if( ( _frameCounter >= _fireRate ) && isMouseDown){
        fire(direction, position, bullets);
        _frameCounter = 0;
    }
}

void Gun::fire(glm::vec2 direction, const glm::vec2& position, std::vector<Bullet>& bullets){

    static std::mt19937 eng{std::random_device{}()}; //Create random engine object
    std::uniform_real_distribution<float> realDist(-_spread, _spread); //Create uniform real distribution object


    for(int i = 0; i < _bulletsPerShot; i++){
        bullets.push_back(Bullet(position, glm::rotate(direction, realDist(eng)), _bulletSpeed, _bulletDamage));
    }
}
