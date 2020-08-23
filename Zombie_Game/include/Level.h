#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include <Bengine/include/SpriteBatch.h>

const float TILE_WIDTH = 64.0f;

class Level
{
    public:
        //Load the level in constructor
        Level(const std::string &fileName);
        virtual ~Level();

        void draw();

        //Getters
        glm::vec2 getStartPlayerPos() const{return _startPlayerPos;}
        const std::vector<glm::vec2>& getStartZombiePos() const{return _startZombiePos;}
        const std::vector<std::string>& getLevelData() const{return _levelData;}
        int getNumHumans() const{return _numHumans;}
        int getWidth() const { return _levelData[0].size(); }//Assumming the map is squared
        int getHeight() const { return _levelData.size(); }
        char getTile(glm::ivec2 position) const{ return _levelData[position.y][position.x];}


    private:
        std::vector<std::string> _levelData;
        int _numHumans;

        Bengine::SpriteBatch _spriteBatch;

        glm::vec2 _startPlayerPos;
        std::vector<glm::vec2> _startZombiePos;
};

#endif // LEVEL_H
