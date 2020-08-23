#include "MainGame.h"
#include "Zombie.h"
#include "Gun.h"

#include <Bengine/include/Errors.h>
#include <Bengine/include/Bengine.h>
#include <Bengine/include/Timing.h>

#include <iostream>
#include <string>
#include <random>
#include <assert.h>

MainGame::MainGame() :
    _screenWidth(1024),
    _screenHeight(600),
    _maxFps(60.0f),
    _fps(0.0f),
    _currentLevel(0),
    _player(nullptr),
    _gameState(GameState::PLAY),
    _numHumansKilled(0),
    _numZombiesKilled(0)
{

}

MainGame::~MainGame(){

    //Free level memory
    for(auto it = _levels.begin(); it != _levels.end(); it++){
        delete *it;
    }

    //Delete remaining humans
    for(auto it = _humans.begin(); it != _humans.end(); it++){
        delete *it;
    }

    //Delete remaining zombies
    for(auto it = _zombies.begin(); it != _zombies.end(); it++){
        delete *it;
    }
}

//Runs the game
void MainGame::run(){
    initSystems();
    initLevel();



    //Returns when game ends
    gameLoop();

}

//Initialize SDL and OpenGL and anything needed
void MainGame::initSystems(){
    Bengine::init();

    _window.create("ZombieGame", _screenWidth, _screenHeight, 0);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f); //Set background window color

    initShaders();

    _agentSpriteBatch.init();

    _camera.init(_screenWidth, _screenHeight);


}

void MainGame::initLevel(){
    //Set map
    _levels.push_back(new Level("Levels/level1.txt"));
    _currentLevel = 0;

    //Set player
    _player = new Player();
    _player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);
    _humans.push_back(_player);


    //Set guns
    _player->addGun(new Gun("Magnum", 30, 1, glm::radians(5.0f), 30.0f, BULLET_SPEED));
    _player->addGun(new Gun("Shotgun", 60, 60, glm::radians(20.0f), 4.0f, BULLET_SPEED));
    _player->addGun(new Gun("M25", 5, 1, glm::radians(7.0f), 150.0f, BULLET_SPEED));

    //Set Humans

    static std::mt19937 eng{std::random_device{}()};
    std::uniform_int_distribution<int> randPosX(2, (_levels[_currentLevel]->getWidth() - 2));
    std::uniform_int_distribution<int> randPosY(2, (_levels[_currentLevel]->getHeight() - 2));

    for(int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++){
        _humans.push_back(new Human);
        _humans.back()->init(HUMAN_SPEED, glm::vec2(randPosX(eng), randPosY(eng)) * TILE_WIDTH);
    }

    //Set Zombies
    const auto zombiePos = _levels[_currentLevel]->getStartZombiePos();
    for( auto it = zombiePos.begin(); it != zombiePos.end(); it++ ){
        _zombies.push_back(new Zombie);
        _zombies.back()->init(ZOMBIE_SPEED, *it);

    }

    //Set camera
    _camera.setScale(CAMERA_SCALE);

}

void MainGame::initShaders(){
    _textureProgram.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();

}

void MainGame::gameLoop(){

    Bengine::FpsLimiter fpsLimiter;
    fpsLimiter.setMaxFPS(_maxFps);

    while(_gameState == GameState::PLAY){
        fpsLimiter.begin();

        checkVictory();

        _inputManager.update();

        processInput();

        updateAgents();

        updateBullets();

        _camera.setPosition(_player->getPosition());

        _camera.update();

//    int a;
//    std::cin >> a;
//
//    _levels[0]->draw();

        drawGame();

        _fps = fpsLimiter.end();
        //std::cout << _fps << std::endl;

    }
}

void MainGame::updateAgents(){
    //Update humans
    for(auto it = _humans.begin(); it != _humans.end(); it++){
        (*it)->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);

    }
    //Update Zombies
    for(auto it = _zombies.begin(); it != _zombies.end(); it++){
        (*it)->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);

    }

    ///Collide Agents

    //Update Human Collisions
    for(auto it = _humans.begin(); it != _humans.end(); it++){
        for(auto jit = it + 1; jit != _humans.end(); jit++){
            (*it)->collideWithAgent(*jit);
        }
    }

    //Update Zombie Collisions
    for(int i = 0; i < _zombies.size(); i++){
        //Collide with zombies
        for(int j = i + 1; j < _zombies.size(); j++){
            _zombies[i]->collideWithAgent(_zombies[j]);
        }
        //Collide with Humans
        for(int j = 1; j < _humans.size(); j++){
            if(_zombies[i]->collideWithAgent(_humans[j])){
                /// Push_back invalidates vector iterators, so indexes are better used to iterate through the loop
                _zombies.push_back(new Zombie);
                _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());

                delete _humans[j];

                //_humans.erase(jit--);
                _humans[j] = _humans.back();
                _humans.pop_back();
                j--;
            }

        }
        //Collide With Player
        if(_zombies[i]->collideWithAgent(_humans[0])){
            Bengine::fatalError("GAME OVER");
        }
    }
//    //Update Zombie Collisions
//    for(auto it = _zombies.begin(); it != _zombies.end(); it++){
//        //Collide with zombies
//        if(
//        for(auto jit = (it + 1); jit != _zombies.end(); jit++){
//            (*it)->collideWithAgent(*jit);
//        }
//        //Collide with Humans
//        for(auto kit = _humans.begin(); kit != _humans.end(); kit++){
//            if((*it)->collideWithAgent(*kit)){
//                _zombies.push_back(new Zombie);
//                _zombies.back()->init(ZOMBIE_SPEED, (*kit)->getPosition());
//
//                delete (*kit);
//
//                //_humans.erase(jit--);
//                (*kit) = _humans.back();
//                _humans.pop_back();
//                kit--;
//            }
//
//        }
//    }
}

void MainGame::updateBullets(){

    //Iterate through bullets
    for(auto bit = _bullets.begin(); bit != _bullets.end();){///< bit -> bullet iterator

        bool nextBulletFlag = false;///<Allows jump to next iteration when collision occurs

        //Check wall collision
        if((*bit).update(_levels[_currentLevel]->getLevelData())){
            bit = _bullets.erase(bit);
            continue;
        }
        //Check zombie Collisions
        for(auto jit = _zombies.begin(); jit != _zombies.end(); jit++){
            if((*bit).collideWithAgent(*jit)){

                //Apply damage to zombie and check death
                if((*jit)->receiveDamage((*bit).getDamage())){
                    delete (*jit);
                    jit = _zombies.erase(jit);
                    _numZombiesKilled++;
                }

                //Remove bullet
                bit = _bullets.erase(bit);
                nextBulletFlag = true;
                break;
            }
        }
        if(nextBulletFlag) continue;

        //Check human collitions
        for(auto jit = _humans.begin() + 1; jit != _humans.end(); jit++){
            if((*bit).collideWithAgent(*jit)){

                //Apply damage to human and check death
                if((*jit)->receiveDamage((*bit).getDamage())){
                    delete (*jit);
                    jit = _humans.erase(jit);
                    _numHumansKilled++;
                }

                //Remove bullet
                bit = _bullets.erase(bit);
                nextBulletFlag = true;
                break;
            }
        }
        if(nextBulletFlag) continue;
        bit++;
    }

//    for(int i = 0; i < _bullets.size(); i++){
//        if(_bullets[i].update(_humans, _zombies, _levels[_currentLevel]->getLevelData())){
//            _bullets.erase(_bullets.begin() + i);
//            i--;
//        }
//    }
}

void MainGame::processInput(){
    SDL_Event evnt;

    while(SDL_PollEvent(&evnt)){
        switch(evnt.type){
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

}

void MainGame::checkVictory(){
    if(_zombies.empty()){

        int humansLeft = _humans.size();
        if(humansLeft == 1){
            humansLeft = 0;
        }else{
            humansLeft--;
        }

        printf("***Victory!***\nZombies Killed: %d\nHumans Killed: %d\n", _numZombiesKilled, _numHumansKilled);
        printf("There are %d/%d civilians remaining.", humansLeft, _levels[_currentLevel]->getNumHumans());
        Bengine::fatalError("a");
    }
}

void MainGame::drawGame(){
    //Set base depth to 1.0
    glClearDepth(1.0);

    //Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Enable shader
    _textureProgram.use();

    ///Draw code goes Here
    glActiveTexture(GL_TEXTURE0);

    //Make sure shader uses texture 0
    GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    //Get camera matrix and upload to shaders
    glm::mat4 projectionMatrix = _camera.getCameraMatrix();
    GLint pUniform = _textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    //Draw Level
    _levels[_currentLevel]->draw();

    //Draw Agents
    _agentSpriteBatch.begin();

    //Draw Humans
    for(auto it = _humans.begin(); it != _humans.end(); it++){
        (*it)->draw(_agentSpriteBatch);
    }

    //Draw Bullets
    for(auto it = _bullets.begin(); it != _bullets.end(); it++){
        (*it).draw(_agentSpriteBatch);
    }

    //Draw Zombies
    for(auto it = _zombies.begin(); it != _zombies.end(); it++){
        (*it)->draw(_agentSpriteBatch);
    }

    _agentSpriteBatch.end();

    _agentSpriteBatch.renderBatch();
    //Finishes drawnig humans

    _textureProgram.unuse();

    //Swap our buffer and draw everything to the screen
    _window.swapBuffer();
}

