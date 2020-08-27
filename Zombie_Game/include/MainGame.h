#ifndef MAINGAME_H
#define MAINGAME_H

#include <SDL2/SDL.h>
#include <GLEW/glew.h>
#include <vector>

#include "Level.h"
#include "Player.h" ///< Also includes "Human.h" and "Agent.h"
#include "Bullet.h"

#include <Bengine/include/GLTexture.h>
#include <Bengine/include/GLSLProgram.h>
#include <Bengine/include/Sprite.h>
#include <Bengine/include/Window.h>
#include <Bengine/include/Camera2D.h>
#include <Bengine/include/SpriteBatch.h>
#include <Bengine/include/InputManager.h>
#include <Bengine/include/SpriteFont.h>
#include <Bengine/include/AudioEngine.h>
#include <Bengine/include/ParticleBatch2D.h>
#include <Bengine/include/ParticleEngine2D.h>

class Zombie;

enum class GameState{PLAY, EXIT};

const float HUMAN_SPEED = 1.00f;
const float ZOMBIE_SPEED = 1.30f;
const float PLAYER_SPEED = 5.0f;
const float BULLET_SPEED = 20.0f;

const float CAMERA_SCALE = 1.0f;

class MainGame
{
    public:
        MainGame();
        virtual ~MainGame();

        void run();


    private:
        void initSystems();
        void initLevel();
        void initShaders();
        void gameLoop();
        void updateAgents(float deltaTime);
        void updateBullets(float deltaTime);///< Also manages zombie and human deaths
        void processInput();
        void checkVictory();

        //Renders Game
        void drawGame();
        void drawHUD();
        ///Adds blood effect
        void drawBlood(const glm::vec2& position, const glm::vec2& direction, int numParticles);

        int m_screenWidth = 1024;
        int m_screenHeight = 600;
        float m_maxFps = 60.0f;
        float m_fps = 0.0f;
        int m_currentLevel = 0;

        Bengine::Window m_window;

        Bengine::GLSLProgram m_textureProgram; ///< Shader Program

        Bengine::InputManager m_inputManager;

        Bengine::Camera2D m_camera;
        Bengine::Camera2D m_HUDcamera;

        Bengine::SpriteBatch m_agentSpriteBatch;
        Bengine::SpriteBatch m_hudSpriteBatch;

        Bengine::ParticleEngine2D m_particleEngine;
        Bengine::ParticleBatch2D* m_bloodParticleBatch;

        Bengine::SpriteFont* m_spriteFont;

        Bengine::AudioEngine m_audioEngine;

        std::vector<Level*> m_levels;

        Player* m_player = nullptr;
        std::vector<Human*> m_humans;
        std::vector<Zombie*> m_zombies;
        std::vector<Bullet> m_bullets;

        GameState m_gameState = GameState::PLAY;

        int m_numHumansKilled = 0;
        int m_numZombiesKilled = 0;

};

#endif // MAINGAME_H
