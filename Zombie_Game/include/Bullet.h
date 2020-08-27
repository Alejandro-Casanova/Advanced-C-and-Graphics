#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include <Bengine/include/SpriteBatch.h>
#include <vector>
#include <string>

class Human;
class Zombie;
class Agent;

const float BULLET_RADIUS = 5.0f;

class Bullet
{
    public:
        Bullet(glm::vec2 pos, glm::vec2 dir, float speed, /*int lifeTime, */int damage);
        virtual ~Bullet();

        bool update(const std::vector<std::string>& levelData, float deltaTime); //Returns true when lifetime is over
        void draw(Bengine::SpriteBatch& spriteBatch) const;
        bool collideWithWorld(const std::vector<std::string>& levelData) const;
        bool collideWithAgent(const Agent* agent) const;

        //Getters
        int getDamage() const { return _damage; }
        glm::vec2 getPosition() const { return _position; }
        glm::vec2 getDirection() const { return _direction; }


    protected:

    private:


        //int _lifeTime;
        float _speed;
        glm::vec2 _direction;
        glm::vec2 _position;
        int _damage;
};

#endif // BULLET_H
