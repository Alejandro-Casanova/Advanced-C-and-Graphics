#include "Bullet.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

#include <Bengine/include/ResourceManager.h>
#include <Bengine/include/GLTexture.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed,
                                              //  int lifeTime,
                                                int damage) :

    _speed (speed),
    _direction(dir),
    _position(pos),
    //_lifeTime(lifeTime),
    _damage(damage){

}

Bullet::~Bullet()
{
    //dtor
}

bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime){
    _position += _direction * _speed * deltaTime;
//    if(--_lifeTime == 0){
//        return true;
//    }
    if( collideWithWorld(levelData) ){
        return true;
    }

    return false;
}

void Bullet::draw(Bengine::SpriteBatch& spriteBatch) const{

    glm::vec4 posAndSize = glm::vec4(_position.x - BULLET_RADIUS,
                                     _position.y - BULLET_RADIUS,
                                     2 * BULLET_RADIUS,
                                     2 * BULLET_RADIUS);

    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    Bengine::ColorRGBA8 color(255,255, 255, 255);
    static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("textures/circle.png");

    spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::collideWithAgent(const Agent* agent) const{

    const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

    glm::vec2 centerPosA = _position;
    glm::vec2 centerPosB = agent->getPosition() + AGENT_RADIUS;

    glm::vec2 distVec = centerPosA - centerPosB;

    float collisionDepth = MIN_DISTANCE - glm::length(distVec);

    if(collisionDepth > 0){

        return true;
    }

    return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) const{
    glm::ivec2 position(round( (_position.x - TILE_WIDTH / 2 ) / TILE_WIDTH), round( (_position.y - TILE_WIDTH / 2 ) / TILE_WIDTH) );

    //If outside the world, just return
    if(position.x < 0 || position.x > levelData[0].size() ||
       position.y < 0 || position.y > levelData.size()){
       return true;
    }

    if( levelData[position.y][position.x] != '.' ){
        return true;
    }
    return false;


}

