#ifndef AGENT_H
#define AGENT_H

#include <string>

#include <glm/glm.hpp>
#include <Bengine/include/GLTexture.h>
#include <Bengine/include/SpriteBatch.h>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

//Prevents circular includes
class Zombie;
class Human;

///Agent is an abstract class
class Agent
{
    public:
        Agent();
        virtual ~Agent();

        virtual void update(const std::vector<std::string>& levelData,
                            std::vector<Human*>& humans,
                            std::vector<Zombie*>& zombies,
                            float deltaTime) = 0;

        bool collideWithLevel(const std::vector<std::string>& levelData);
        bool collideWithAgent(Agent* agent);

        void draw(Bengine::SpriteBatch& spriteBatch);

        virtual bool receiveDamage(int damageVal); //Return true if death

        glm::vec2 getPosition() const{ return m_position; }
        glm::vec2 getDirection() const{ return m_direction; }


    protected:
        void checkTilePosition(const std::vector<std::string>& levelData,
                               std::vector<glm::vec2>& collideTilePositions,
                                float x, float y);
        static bool compareTiles(const glm::vec2& tile1, const glm::vec2& tile2);
        void sortTiles(std::vector<glm::vec2>& collideTilePositions);
        void collideWithTile(glm::vec2 tilePos);

        glm::vec2 m_position;
        glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
        float m_speed;
        Bengine::ColorRGBA8 m_color;
        float m_health;
        GLuint m_textureID;


};

#endif // AGENT_H
