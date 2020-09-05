#ifndef SCREENLIST_H
#define SCREENLIST_H

#include <vector>



namespace Bengine{

class IMainGame;
class IGameScreen;

class ScreenList
{
    public:
        ScreenList(IMainGame* game);
        virtual ~ScreenList();

        IGameScreen* moveNext();
        IGameScreen* movePrevious();

        void setScreen(int nextScreen);
        void addScreen(IGameScreen* newScreen);

        void destroy();

        IGameScreen* getCurrent();

    protected:
        std::vector<IGameScreen*> m_screens;
        int m_currentScreenIndex = -1;
        IMainGame* m_game = nullptr;

    private:
};

}

#endif // SCREENLIST_H
