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

class Zombie;

enum class GameState{PLAY, EXIT};

const float HUMAN_SPEED = 1.00f;
const float ZOMBIE_SPEED = 1.30f;
const float PLAYER_SPEED = 10.0f;
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
        void updateAgents();
        void updateBullets();///< Also manages zombie and human deaths
        void processInput();
        void checkVictory();

        //Renders Game
        void drawGame();

        int _screenWidth;
        int _screenHeight;
        float _maxFps;
        float _fps;
        int _currentLevel;

        Bengine::Window _window;

        Bengine::GLSLProgram _textureProgram; ///< Shader Program

        Bengine::InputManager _inputManager;

        Bengine::Camera2D _camera;

        Bengine::SpriteBatch _agentSpriteBatch;

        std::vector<Level*> _levels;

        Player* _player;
        std::vector<Human*> _humans;
        std::vector<Zombie*> _zombies;
        std::vector<Bullet> _bullets;

        GameState _gameState;

        int _numHumansKilled;
        int _numZombiesKilled;

};

#endif // MAINGAME_H
