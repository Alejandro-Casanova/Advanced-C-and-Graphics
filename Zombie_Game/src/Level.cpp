#include "Level.h"

#include <Bengine/include/Errors.h>
#include <Bengine/include/ResourceManager.h>
#include <fstream>
#include <iostream>

Level::Level(const std::string &fileName) : _numHumans(0), _startPlayerPos(0.0f) {

    std::ifstream file;
    file.open(fileName);

    //Error check
    if(file.fail()){
        Bengine::fatalError("Could not load level file: " + fileName + "\n");
    }

    //Read the number of humans (Dismiss first string "Humans: " and get only the number)
    std::string temp;
    file >> temp >> _numHumans;
    std::getline(file, temp); ///< Throw away first line

    //Read level data
    while(std::getline(file, temp)){
        _levelData.emplace_back(temp);
    }

    _spriteBatch.init();
    _spriteBatch.begin();

    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f); //UV rect for all textures
    Bengine::ColorRGBA8 whiteColor(255, 255, 255, 255);

    //Render all tiles
    for(auto yit = _levelData.begin(); yit != _levelData.end(); yit++){
        static int y = 0;
        int x = 0;

        for(auto xit = (*yit).begin(); xit != (*yit).end(); xit++){

            char tile =_levelData[y][x];
            glm::vec4 destRect(x * TILE_WIDTH, /*(_levelData.size() - 1 - y)*/y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH); ///< Get destination rectangle for current tile sprite

            switch(tile){
            case '.':

                break;
            case 'B':
            case 'R':
                _spriteBatch.draw(destRect,
                                  uvRect,
                                  Bengine::ResourceManager::getTexture("textures/red_bricks.png").id,
                                  0.0f,
                                  whiteColor);
                break;
            case 'L':
                _spriteBatch.draw(destRect,
                                  uvRect,
                                  Bengine::ResourceManager::getTexture("textures/light_bricks.png").id,
                                  0.0f,
                                  whiteColor);
                break;
            case 'G':
                _spriteBatch.draw(destRect,
                                  uvRect,
                                  Bengine::ResourceManager::getTexture("textures/glass.png").id,
                                  0.0f,
                                  whiteColor);
                break;
            case '@':
                _startPlayerPos = glm::vec2(x * TILE_WIDTH, /*(_levelData.size() - 1 - y)*/y * TILE_WIDTH);
                _levelData[y][x] = '.';//Prevents collision in the future
                break;
            case 'Z':
                _startZombiePos.emplace_back(x * TILE_WIDTH, /*(_levelData.size() - 1 - y)*/y * TILE_WIDTH);
                _levelData[y][x] = '.';//Prevents collision in the future
                break;
            default:
                printf("Unexpected symbol: '%c' at (%d, %d) in file <%s>\n", tile, x, y, fileName.c_str());
                Bengine::fatalError("Could not draw map because of the above stated.\n");
                    break;
            }
            x++;

        }
        y++;

    }


    _spriteBatch.end();
}

Level::~Level()
{
    //dtor
}

void Level::draw(){
    _spriteBatch.renderBatch();
}
