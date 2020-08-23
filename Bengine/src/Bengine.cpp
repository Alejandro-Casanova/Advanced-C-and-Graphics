#include <SDL2/SDL.h>
#include <GLEW/glew.h>

namespace Bengine{

    int init(){

        //Initialize SDL
        SDL_Init(SDL_INIT_EVERYTHING);

        //Set double buffered window to prevent flickering
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        return 0;
    }

}
