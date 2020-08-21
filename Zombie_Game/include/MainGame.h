#ifndef MAINGAME_H
#define MAINGAME_H

#include <SDL2/SDL.h>
#include <GLEW/glew.h>
#include <vector>

#include "Bullet.h"

#include <Bengine/include/GLTexture.h>
#include <Bengine/include/GLSLProgram.h>
#include <Bengine/include/Sprite.h>
#include <Bengine/include/Window.h>
#include <Bengine/include/Bengine.h>
#include <Bengine/include/Camera2D.h>
#include <Bengine/include/SpriteBatch.h>
#include <Bengine/include/InputManager.h>
#include <Bengine/include/Timing.h>

enum class GameState{PLAY, EXIT};

class MainGame
{
    public:
        MainGame();
        virtual ~MainGame();

        void run();


    private:
        void initSystems();
        void initShaders();
        void gameLoop();
        void processInput();

        //Renders Game
        void drawGame();

        Bengine::Window _window;
        int _screenWidth;
        int _screenHeight;
        GameState _gameState;

        float _fps;
        float _maxFps;

        std::vector<Bengine::Sprite*> _sprites;

        Bengine::GLSLProgram _colorProgram;

        Bengine::Camera2D _camera;

        Bengine::SpriteBatch _spriteBatch;

        Bengine::InputManager _inputManager;

        Bengine::FpsLimiter _fpsLimiter;

        std::vector<Bullet> _bullets;


};

#endif // MAINGAME_H
