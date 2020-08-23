#ifndef GUN_H
#define GUN_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Bullet.h"


class Gun
{
    public:
        Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
        virtual ~Gun();

        void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);

    protected:

    private:

        void fire(glm::vec2 direction, const glm::vec2& position, std::vector<Bullet>& bullets);

        std::string _name;
        int _fireRate;
        int _bulletsPerShot;
        float _spread; ///<Accuracy
        float _bulletSpeed;
        float _bulletDamage;
        int _frameCounter;
};

#endif // GUN_H
