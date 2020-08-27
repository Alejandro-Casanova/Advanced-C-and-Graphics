#include "MainGame.h"
#include "Zombie.h"
#include "Gun.h"

#include <Bengine/include/BengineErrors.h>
#include <Bengine/include/Bengine.h>
#include <Bengine/include/Timing.h>
#include <Bengine/include/ResourceManager.h>

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <string>
#include <random>
#include <assert.h>
#include <sstream>

MainGame::MainGame(){}

MainGame::~MainGame(){

    //Free level memory
    for(auto it = m_levels.begin(); it != m_levels.end(); it++){
        delete *it;
    }

    //Delete remaining humans
    for(auto it = m_humans.begin(); it != m_humans.end(); it++){
        delete *it;
    }

    //Delete remaining zombies
    for(auto it = m_zombies.begin(); it != m_zombies.end(); it++){
        delete *it;
    }

    m_audioEngine.destroy();
}

//Runs the game
void MainGame::run(){
    initSystems();
    initLevel();

    ///Load and play music
    Bengine::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
    music.play(-1); //Play in infinite loop


    //Returns when game ends
    gameLoop();

}

//Initialize SDL and OpenGL and anything needed
void MainGame::initSystems(){
    Bengine::init();

    m_audioEngine.init();

    m_window.create("ZombieGame", m_screenWidth, m_screenHeight, 0);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f); //Set background window color

    initShaders();

    ///Initialize Sprite Batches
    m_agentSpriteBatch.init();
    m_hudSpriteBatch.init();

    ///Initialize SpriteFont (Must be initialized after openGL and SDL)
    m_spriteFont = new Bengine::SpriteFont("Fonts/zombiecontrol.ttf", 64);

    ///Initialize Cameras
    m_camera.init(m_screenWidth, m_screenHeight);
    m_HUDcamera.init(m_screenWidth, m_screenHeight);

    m_camera.setScale(CAMERA_SCALE);
    m_HUDcamera.setScale(CAMERA_SCALE);
    m_HUDcamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));
    //m_HUDcamera.setPosition(glm::vec2(0.0f));

    ///Initialize Particle Batches
    m_bloodParticleBatch = new Bengine::ParticleBatch2D();
    m_bloodParticleBatch->init(1000, 0.03f,
                               Bengine::ResourceManager::getTexture("textures/particle.png"),
                               [](Bengine::Particle2D& particle, float deltaTime){
        particle.position += particle.velocity * deltaTime;
        particle.color.a = (GLubyte)(particle.life * 255.0f);
    });

    m_particleEngine.addParticleBatch(m_bloodParticleBatch);


}

void MainGame::initLevel(){
    //Set map
    m_levels.push_back(new Level("Levels/level1.txt"));
    m_currentLevel = 0;

    //Set player
    m_player = new Player();
    m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets);
    m_humans.push_back(m_player);


    //Set guns
    m_player->addGun(new Gun("Magnum", 30, 1, glm::radians(5.0f), 75.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/Shots/pistol.wav", MIX_MAX_VOLUME / 8)));
    m_player->addGun(new Gun("Shotgun", 60, 20, glm::radians(20.0f), 10.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/Shots/shotgun.wav", MIX_MAX_VOLUME / 8)));
    m_player->addGun(new Gun("M25", 5, 1, glm::radians(7.0f), 30.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/Shots/cg1.wav", MIX_MAX_VOLUME / 8)));

    //Set Humans
    static std::mt19937 eng{std::random_device{}()};
    std::uniform_int_distribution<int> randPosX(2, (m_levels[m_currentLevel]->getWidth() - 2));
    std::uniform_int_distribution<int> randPosY(2, (m_levels[m_currentLevel]->getHeight() - 2));

    for(int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++){
        m_humans.push_back(new Human);
        m_humans.back()->init(HUMAN_SPEED, glm::vec2(randPosX(eng), randPosY(eng)) * TILE_WIDTH);
    }

    //Set Zombies
    const auto zombiePos = m_levels[m_currentLevel]->getStartZombiePos();
    for( auto it = zombiePos.begin(); it != zombiePos.end(); it++ ){
        m_zombies.push_back(new Zombie);
        m_zombies.back()->init(ZOMBIE_SPEED, *it);

    }



}

void MainGame::initShaders(){
    m_textureProgram.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();

}

void MainGame::gameLoop(){

    Bengine::FpsLimiter fpsLimiter;
    fpsLimiter.setMaxFPS(m_maxFps);

    const float DESIREDm_fps = 60.0f;
    const int MAX_PHYSICS_STEPS = 6; //Max steps simulated in one frame. Prevents "spiral of death", where too many steps prevent the program from rendering
    const float MS_PER_SECOND = 1000.0f;
    const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIREDm_fps;
    const float MAX_DELTA_TIME = 1.0f;

    float prevTicks = SDL_GetTicks();

    while(m_gameState == GameState::PLAY){
        fpsLimiter.begin();

        float newTicks = SDL_GetTicks();
        float frameTime = newTicks - prevTicks;
        prevTicks = newTicks;
        float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;

        checkVictory();

        m_inputManager.update();

        processInput();

        int i = 0;//Prevents "spiral of death"
        while( ( totalDeltaTime > 0.0f ) && ( i < MAX_PHYSICS_STEPS ) ){
            float deltaTime = std::min(MAX_DELTA_TIME, totalDeltaTime);
            updateAgents(deltaTime);
            updateBullets(deltaTime);
            m_particleEngine.update(deltaTime);
            i++;
            totalDeltaTime -= deltaTime;
        }

        m_camera.setPosition(m_player->getPosition());
        m_camera.update();
        m_HUDcamera.update();

//    int a;
//    std::cin >> a;
//
//    m_levels[0]->draw();

        drawGame();

        m_fps = fpsLimiter.end();
        //std::cout << m_fps << std::endl;

    }
}

void MainGame::updateAgents(float deltaTime){
    //Update humans
    for(auto it = m_humans.begin(); it != m_humans.end(); it++){
        (*it)->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);

    }
    //Update Zombies
    for(auto it = m_zombies.begin(); it != m_zombies.end(); it++){
        (*it)->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);

    }

    ///Collide Agents

    //Update Human Collisions
    for(auto it = m_humans.begin(); it != m_humans.end(); it++){
        for(auto jit = it + 1; jit != m_humans.end(); jit++){
            (*it)->collideWithAgent(*jit);
        }
    }

    //Update Zombie Collisions
    for(int i = 0; i < m_zombies.size(); i++){
        //Collide with zombies
        for(int j = i + 1; j < m_zombies.size(); j++){
            m_zombies[i]->collideWithAgent(m_zombies[j]);
        }
        //Collide with Humans
        for(int j = 1; j < m_humans.size(); j++){
            if(m_zombies[i]->collideWithAgent(m_humans[j])){

                if(m_humans[j]->receiveInfection(5)){


                    /// Push_back invalidates vector iterators, so indexes are better used to iterate through the loop
                    m_zombies.push_back(new Zombie);
                    m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition(), m_humans[j]->getDirection());

                    delete m_humans[j];

                    //m_humans.erase(jit--);
                    m_humans[j] = m_humans.back();
                    m_humans.pop_back();
                    j--;

                }
            }

        }
        //Collide With Player
        if(m_zombies[i]->collideWithAgent(m_humans[0])){
            if(m_humans[0]->receiveInfection(5)){
                Bengine::fatalError("GAME OVER\n");
            }
        }
    }

//    //Update Zombie Collisions
//    for(auto it = m_zombies.begin(); it != m_zombies.end(); it++){
//        //Collide with zombies
//        if(
//        for(auto jit = (it + 1); jit != m_zombies.end(); jit++){
//            (*it)->collideWithAgent(*jit);
//        }
//        //Collide with Humans
//        for(auto kit = m_humans.begin(); kit != m_humans.end(); kit++){
//            if((*it)->collideWithAgent(*kit)){
//                m_zombies.push_back(new Zombie);
//                m_zombies.back()->init(ZOMBIE_SPEED, (*kit)->getPosition());
//
//                delete (*kit);
//
//                //m_humans.erase(jit--);
//                (*kit) = m_humans.back();
//                m_humans.pop_back();
//                kit--;
//            }
//
//        }
//    }

}

void MainGame::updateBullets(float deltaTime){

    //Iterate through bullets
    for(auto bit = m_bullets.begin(); bit != m_bullets.end();){///< bit -> bullet iterator

        bool nextBulletFlag = false;///<Allows jump to next iteration when collision occurs

        //Check wall collision
        if( (*bit).update( m_levels[m_currentLevel]->getLevelData(), deltaTime ) ){
            bit = m_bullets.erase(bit);
            continue;
        }
        //Check zombie Collisions
        for(auto jit = m_zombies.begin(); jit != m_zombies.end(); jit++){
            if((*bit).collideWithAgent(*jit)){

                ///Apply blood effect
                drawBlood((*bit).getPosition(), (*bit).getDirection(), 10);

                ///Apply damage to zombie and check death
                if((*jit)->receiveDamage((*bit).getDamage())){
                    delete (*jit);
                    jit = m_zombies.erase(jit);
                    m_numZombiesKilled++;
                }

                //Remove bullet
                bit = m_bullets.erase(bit);
                nextBulletFlag = true;
                break;
            }
        }
        if(nextBulletFlag) continue;

        //Check human collitions
        for(auto jit = m_humans.begin() + 1; jit != m_humans.end(); jit++){
            if((*bit).collideWithAgent(*jit)){

                ///Apply blood effect
                drawBlood((*bit).getPosition(), (*bit).getDirection(), 10);

                //Apply damage to human and check death
                if((*jit)->receiveDamage((*bit).getDamage())){
                    delete (*jit);
                    jit = m_humans.erase(jit);
                    m_numHumansKilled++;
                }

                //Remove bullet
                bit = m_bullets.erase(bit);
                nextBulletFlag = true;
                break;
            }
        }
        if(nextBulletFlag) continue;
        bit++;
    }

//    for(int i = 0; i < m_bullets.size(); i++){
//        if(m_bullets[i].update(m_humans, m_zombies, m_levels[m_currentLevel]->getLevelData())){
//            m_bullets.erase(m_bullets.begin() + i);
//            i--;
//        }
//    }
}

void MainGame::processInput(){
    SDL_Event evnt;

    while(SDL_PollEvent(&evnt)){
        switch(evnt.type){
            case SDL_QUIT:
                m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

}

void MainGame::checkVictory(){
    if(m_zombies.empty()){

        int humansLeft = m_humans.size();
        if(humansLeft == 1){
            humansLeft = 0;
        }else{
            humansLeft--;
        }

        printf("***Victory!***\nZombies Killed: %d\nHumans Killed: %d\n", m_numZombiesKilled, m_numHumansKilled);
        printf("There are %d/%d civilians remaining.", humansLeft, m_levels[m_currentLevel]->getNumHumans());
        Bengine::fatalError("");
    }
//    if(m_humans[0]->receiveInfection(0)){
//        Bengine::fatalError("GAME OVER");
//    }
}

void MainGame::drawGame(){
    //Set base depth to 1.0
    glClearDepth(1.0);

    //Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Enable shader
    m_textureProgram.use();

    ///Draw code goes Here
    glActiveTexture(GL_TEXTURE0);

    //Make sure shader uses texture 0
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    //Get camera matrix and upload to shaders
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    //Draw Level
    m_levels[m_currentLevel]->draw();

    ///Draw Agents
    m_agentSpriteBatch.begin();

    //Draw Humans
    for(auto& it : m_humans){
        //Draw only if in view
        if(m_camera.isBoxInView(it->getPosition(), glm::vec2(AGENT_WIDTH))){
            it->draw(m_agentSpriteBatch);
        }

    }

    //Draw Bullets
    for(auto& it : m_bullets){
        if(m_camera.isBoxInView(it.getPosition(), glm::vec2(2 * BULLET_RADIUS))){
            it.draw(m_agentSpriteBatch);
        }
    }

    //Draw Zombies
    for(auto& it : m_zombies){
        if(m_camera.isBoxInView(it->getPosition(), glm::vec2(AGENT_WIDTH))){
            it->draw(m_agentSpriteBatch);
        }
    }



    m_agentSpriteBatch.end();

    m_agentSpriteBatch.renderBatch();
    //Finishes drawnig humans

    //Render particles
    m_particleEngine.draw(&m_agentSpriteBatch);

    drawHUD();

    m_textureProgram.unuse();

    //Swap our buffer and draw everything to the screen
    m_window.swapBuffer();
}
void MainGame::drawHUD(){
//    char buffer[300];
//    sprintf_s(buffer, "Num humans: %d", m_humans.size());

    std::stringstream buffer;

//    const char* buffer = Buffer.str().c_str();

    //Get camera matrix and upload to shaders
    glm::mat4 projectionMatrix = m_HUDcamera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_hudSpriteBatch.begin();

    //Civilian Counter
    buffer << "Civilians left: " << m_humans.size() - 1;
    m_spriteFont->draw(m_hudSpriteBatch, buffer.str().c_str(),
                      glm::vec2(1.0f, 0.0f), glm::vec2(0.5f), 0.0f,
                      Bengine::ColorRGBA8(255, 255, 255, 255));

    //Zombie Counter
    buffer.str(std::string());//Clear buffer
    buffer << "Zombies left: " << m_zombies.size();
    m_spriteFont->draw(m_hudSpriteBatch, buffer.str().c_str(),
                      glm::vec2(1.0f, 32.0f), glm::vec2(0.5f), 0.0f,
                      Bengine::ColorRGBA8(255, 255, 255, 255));

    //Weapon Selection
    buffer.str(std::string());//Clear buffer

    int i = 0;
    int currentGun = m_player->getCurrentGun();

    for(auto& it : m_player->getGuns()){

        if (i == currentGun){
            buffer << '>';
        }else{
            buffer << ' ';
        }
        i++;
        buffer <<  it->getName() << std::endl;

    }
    buffer << "Equiped Weapons:\n";
    m_spriteFont->draw(m_hudSpriteBatch, buffer.str().c_str(),
                      glm::vec2(1.0f, 96.0f), glm::vec2(0.5f), 0.0f,
                      Bengine::ColorRGBA8(255, 255, 255, 255));

    m_hudSpriteBatch.end();
    m_hudSpriteBatch.renderBatch();
}

void MainGame::drawBlood(const glm::vec2& position, const glm::vec2& direction, int numParticles){

    static std::mt19937 eng{std::random_device{}()}; //Create random engine object
    static std::uniform_real_distribution<float> randAngle(glm::radians(-10.0f), glm::radians(10.0f)); //Create uniform real distribution object
    //static std::uniform_real_distribution<float> randAngle(0.0f, 2 * M_PI); //Create uniform real distribution object
    static std::uniform_real_distribution<float> randVel(1.0f, 5.0f);

    //glm::vec2 velocity(1.0f, 0.0f);
    //glm::vec2 velocity = direction * randVel(eng);

    for(int i = 0; i < numParticles; i++){
        m_bloodParticleBatch->addParticle(position, glm::rotate(direction * randVel(eng), randAngle(eng)),
                                          Bengine::ColorRGBA8(255, 0, 0, 255), 10.0f);
    }
}
