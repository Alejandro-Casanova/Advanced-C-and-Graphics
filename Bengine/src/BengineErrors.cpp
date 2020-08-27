#include "BengineErrors.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>

namespace Bengine{

    //Prints out error message and exits game
    void fatalError(std::string errorString){
        std::cout << errorString << std::endl;
        std::cout << "Enter any key to exit...";
        char tmp;
        std::cin >> tmp;
        SDL_Quit();
        exit(1);
    }

}
