#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Agent.h"

const float ZOMBIE_TURN_SPEED = 0.08f; ///< Radians / frame * deltaTime
const float ZOMBIE_HEALTH = 150.0f;
const GLubyte ZOMBIE_COLOR_R = 100;
const GLubyte ZOMBIE_COLOR_G = 200;
const GLubyte ZOMBIE_COLOR_B = 100;
const GLubyte ZOMBIE_COLOR_A = 255;


class Zombie : public Agent
{
    public:
        Zombie();
        virtual ~Zombie();

        void init(float speed, const glm::vec2& pos);
        void init(float speed, const glm::vec2& pos, const glm::vec2& dir);

        bool receiveDamage(int damageVal) override; //Return true if death

        void update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies,
                            float deltaTime) override;

    protected:

    private:

        Human* getNearestHuman(std::vector<Human*>& humans);
};

#endif // ZOMBIE_H
