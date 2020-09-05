#ifndef HUMAN_H
#define HUMAN_H

#include "Agent.h"
#include "Zombie.h"

const int HUMAN_INFECT_RESIST = 255;
const int HUMAN_TURN_SPEED = 0.08f;

class Gun;

//template<class T> extern int sign(T val); ///<Returns -1 if negative and +1 if positive

class Human : public Agent
{
    public:
        Human();
        virtual ~Human();

        void init(float speed, glm::vec2 pos);

        virtual void update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies,
                            float deltaTime) override;

        virtual bool receiveInfection(int infectVal);

    protected:
        int m_infectionResistance = HUMAN_INFECT_RESIST;
        glm::vec2 m_directionObjective = glm::vec2(1.0f, 0.0f);

    private:
        int m_wallCount = 0; ///< Used to count frames after wall collision. Prevents humans from getting stuck into walls.
};

#endif // HUMAN_H
