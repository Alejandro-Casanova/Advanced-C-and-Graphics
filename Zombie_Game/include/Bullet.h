#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include <Bengine/include/SpriteBatch.h>


class Bullet
{
    public:
        Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
        virtual ~Bullet();

        void draw(Bengine::SpriteBatch& spriteBatch);
        bool update(); //Returns true when lifetime is over

    protected:

    private:
        int _lifeTime;
        float _speed;
        glm::vec2 _direction;
        glm::vec2 _position;
};

#endif // BULLET_H
