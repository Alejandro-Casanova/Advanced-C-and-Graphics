#ifndef APP_H
#define APP_H

#include <Bengine/include/IMainGame.h>

#include "GameplayScreen.h"

class App : public Bengine::IMainGame{
    public:
        App();
        virtual ~App();

        virtual void onInit() override; ///< Called on initialization
        virtual void addScreens() override;
        virtual void onExit() override; ///< Called on exiting

    private:
        std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
};

#endif // APP_H
