#ifndef IMAINGAME_H
#define IMAINGAME_H

#include "Bengine.h"
#include "Window.h"
#include "InputManager.h"
#include "ScreenList.h"

#include <memory>

namespace Bengine{

class IGameScreen;

class IMainGame
{
    public:
        IMainGame();
        virtual ~IMainGame();

        ///Run and initialize the game
        void run();
        void exitGame();

        ///Must be overriden.
        virtual void onInit() = 0; ///< Called on initialization
        virtual void addScreens() = 0;
        virtual void onExit() = 0; ///< Called on exiting

        void onSDLEvent(SDL_Event& evnt);

        const float getFps() const { return m_fps; }

        InputManager inputManager;

    protected:
        virtual void update();
        virtual void draw();

        bool init();
        bool initSystems();

        std::unique_ptr<ScreenList> m_screenList = nullptr;
        IGameScreen* m_currentScreen = nullptr;
        bool m_isRunning = false;
        float m_fps = 0.0f;
        Window m_window;

        int m_screenWidth = 1024;
        int m_screenHeight = 600;

    private:
};

}

#endif // IMAINGAME_H
