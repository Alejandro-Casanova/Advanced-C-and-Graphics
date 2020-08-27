#ifndef GUN_H
#define GUN_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <Bengine/include/AudioEngine.h>

#include "Bullet.h"


class Gun
{
    public:
        Gun(std::string name, int fireRate, int bulletsPerShot,
            float spread, float bulletDamage, float bulletSpeed,
            Bengine::SoundEffect fireEffect);

        virtual ~Gun();

        void update(bool isMouseDown, const glm::vec2& position,
                    const glm::vec2& direction, std::vector<Bullet>& bullets,
                    float deltaTime);

        std::string getName() const {return m_name;}

    protected:

    private:

        void fire(glm::vec2 direction, const glm::vec2& position, std::vector<Bullet>& bullets);

        Bengine::SoundEffect m_fireEffect;
        std::string m_name;
        int m_fireRate;
        int m_bulletsPerShot;
        float m_spread; ///<Accuracy
        float m_bulletSpeed;
        float m_bulletDamage;
        float m_frameCounter = 0;
};

#endif // GUN_H
