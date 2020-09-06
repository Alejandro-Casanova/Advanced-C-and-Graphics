#ifndef IGAMESCREEN_H
#define IGAMESCREEN_H

#define SCREEN_INDEX_NO_SCREEN -1

namespace Bengine{

class IMainGame;

enum class ScreenState{
    NONE,
    RUNNING,
    EXIT_APPLICATION,
    CHANGE_NEXT,
    CHANGE_PREVIOUS
};

class IGameScreen{
public:
    friend class ScreenList;

    IGameScreen(){}
    virtual ~IGameScreen(){}

    ///Returns index of previous or next screen when changing
    virtual int getNextScreenIndex() const = 0;
    virtual int getPreviousScreenIndex() const = 0;

    ///Called at beginning and end of application
    virtual void build() = 0;
    virtual void destroy() = 0;

    ///Called when a screen enters and exits focus
    virtual void onEntry() = 0;
    virtual void onExit() = 0;

    ///Called in the main game loop
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;

    int getScreenIndex() const { return m_screenIndex; }
    ScreenState getScreenState() const { return m_currentState; }
    void setRunning() { m_currentState = ScreenState::RUNNING; }
    void setParentGame(IMainGame* game){ m_game = game; }

protected:
    int m_screenIndex = -1;
    ScreenState m_currentState = ScreenState::NONE;
    IMainGame* m_game = nullptr;
private:


};

}

#endif // IGAMESCREEN_H
