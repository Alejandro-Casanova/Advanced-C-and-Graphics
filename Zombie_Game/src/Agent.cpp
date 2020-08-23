#include "Agent.h"
#include "Level.h"

#include <math.h>
#include <algorithm>

#include <Bengine/include/ResourceManager.h>

Agent::Agent()
{
    //ctor
}

Agent::~Agent()
{
    //dtor
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData){

    std::vector<glm::vec2> collideTilePositions; ///< Will save tile position vectors (relative to the player)

    //Check the four corners
    //First Corner
    checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);

    //Second Corner
    checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y);

    //Third Corner
    checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH);

    //Fourth Corner
    checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

    //Sort tiles to prevent player from getting stuck on walls
    sortTiles(collideTilePositions);

    if(collideTilePositions.size() == 0) return false;

    //Do collision
    for(auto it = collideTilePositions.begin(); it != collideTilePositions.end(); it++){
        collideWithTile(*it);
    }

    return true;
}

bool Agent::collideWithAgent(Agent* agent){

    const float MIN_DISTANCE = AGENT_RADIUS + AGENT_RADIUS;

    glm::vec2 centerPosA = _position + AGENT_RADIUS;
    glm::vec2 centerPosB = agent->getPosition() + AGENT_RADIUS;

    glm::vec2 distVec = centerPosA - centerPosB;

    float collisionDepth = MIN_DISTANCE - glm::length(distVec);

    if(collisionDepth > 0){

        glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth / 2.0f;

        _position += collisionDepthVec;
        agent->_position -= collisionDepthVec;
        return true;
    }

    return false;
}

void Agent::draw(Bengine::SpriteBatch& spriteBatch){

    static int textureID = Bengine::ResourceManager::getTexture("textures/circle.png").id;
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);

    spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}

bool Agent::receiveDamage(int damageVal){
    _health -= damageVal;

    if(_health <= 0){
        return true;
    }
    return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,
                               std::vector<glm::vec2>& collideTilePositions,
                               float x, float y){

    glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
                                    floor(y / (float)TILE_WIDTH));

    //If outside the world, just return
    if(cornerPos.x < 0 || cornerPos.x > levelData[0].size() ||
       cornerPos.y < 0 || cornerPos.y > levelData.size()){
       return;
    }

    //IF tile is collidable, sabes positione vector relative to the player
    if(levelData[cornerPos.y][cornerPos.x] != '.'){
        collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
    }
}

bool Agent::compareTiles(const glm::vec2& tileDist1, const glm::vec2& tileDist2){
    //When two collitions take place at the same time, a bug might take place and the agent may get stuck.
    //By sorting the colliding tiles, this bug is fixed. To sort the vector we must define a function that
    //compares the elements of the vector (glm::vec2)

    return ( glm::length(tileDist1) < glm::length(tileDist2) );

}

void Agent::sortTiles(std::vector<glm::vec2>& collideTilePositions){

    //Sets all positions in reference to the player
    for(auto it = collideTilePositions.begin(); it != collideTilePositions.end(); it++){
        (*it) -= (_position + AGENT_WIDTH / 2.0f);
    }

    std::sort(collideTilePositions.begin(), collideTilePositions.end(), compareTiles);

    //Returns all positions to their original value
    for(auto it = collideTilePositions.begin(); it != collideTilePositions.end(); it++){
        (*it) += (_position + AGENT_WIDTH / 2.0f);
    }
}

void Agent::collideWithTile(glm::vec2 tilePos){

    const float TILE_RADIUS = TILE_WIDTH / 2.0f;
    const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

    glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
    glm::vec2 distVec = centerPlayerPos - tilePos;

    float xDepth = MIN_DISTANCE - abs(distVec.x);
    float yDepth = MIN_DISTANCE - abs(distVec.y);

    //Check for collition
    if(xDepth > 0.0f && yDepth > 0.0f){
        if(std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
            if(distVec.x < 0.0f){
                _position.x -= xDepth;
            }else{
                _position.x += xDepth;
            }
        }else{
            if(distVec.y < 0.0f){
                _position.y -= yDepth;
            }else{
                _position.y += yDepth;
            }
        }
    }




}
