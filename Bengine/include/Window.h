#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <GLEW/glew.h>

#include <string>

namespace Bengine{

    enum windowFlags{INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4};

    class Window
    {
        public:
            Window();
            virtual ~Window();

            int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

            void swapBuffer();

            int getScreenWidth(){return _screenWidth;}
            int getScreenHeight(){return _screenHeight;}

        private:
            SDL_Window* _sdlwindow;
            int _screenWidth;
            int _screenHeight;
    };
}

#endif // WINDOW_H
