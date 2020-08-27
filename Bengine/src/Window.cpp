#include "Window.h"
#include "BengineErrors.h"

namespace Bengine{

Window::Window() : _screenWidth(0), _screenHeight(0)
{
    //ctor
}

Window::~Window()
{
    //dtor
}

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags){

    Uint32 flags = SDL_WINDOW_OPENGL;

    //Check flags
    if(currentFlags & INVISIBLE){
        flags |= SDL_WINDOW_HIDDEN;
    }
    if(currentFlags & FULLSCREEN){
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if(currentFlags & BORDERLESS){
        flags |= SDL_WINDOW_BORDERLESS;
    }

    //Open an SDL Window
    _sdlwindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
    if(_sdlwindow == nullptr){
        fatalError("SDL window could not be created");
    }

    //Set up OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(_sdlwindow);
    if(glContext == nullptr){
        fatalError("SDL_GL context could not be created");
    }

    //Set up glew (optional, but recommended)
    GLenum error = glewInit();
    if(error != GLEW_OK){
        fatalError("Could not initialize glew");
    }

    //Print openGL version
    printf("*** OpenGL version: %s ***\n", glGetString(GL_VERSION));

    //Set background color to blue
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    //Set VYSINC
    SDL_GL_SetSwapInterval(1);

    //Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;

}

void Window::swapBuffer(){
    //Swap our buffer and draw everything to the screen
    SDL_GL_SwapWindow(_sdlwindow);
}

}
