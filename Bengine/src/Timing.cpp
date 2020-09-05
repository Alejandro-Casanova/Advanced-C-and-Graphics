#include "Timing.h"

#include <SDL2/SDL.h>

namespace Bengine{

FpsLimiter::FpsLimiter(){}

void FpsLimiter::init(float maxFPS){
    setMaxFPS(maxFPS);
}

void FpsLimiter::setMaxFPS(float maxFPS){
    _maxFps = maxFPS;
}

void FpsLimiter::begin(){
    _startTicks = SDL_GetTicks();
}

float FpsLimiter::end(){

    calculateFps();

    float frameTicks = (float)(SDL_GetTicks() - _startTicks);
        //Limits the FPS to the max FPS
        if(1000.0f / _maxFps > frameTicks){
            SDL_Delay((Uint32)(1000.0f / _maxFps - frameTicks));
        }
    //float debug = SDL_GetTicks();
    return _fps;
}

void FpsLimiter::calculateFps(){
    ///The number of frames on average
    static const int NUM_SAMPLES = 10;
    ///Stores all the frametimes for each frame that we will average
    static float frameTimes[NUM_SAMPLES];

    static int currentFrame = 0;

    static Uint32 prevTicks = SDL_GetTicks();

    ///Ticks for the current frame
    Uint32 currentTicks = SDL_GetTicks();

    //Calculate the number of ticks (ms) for this frame
    _frameTime = (float)(currentTicks - prevTicks);
    frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

    prevTicks = currentTicks;

    int count;
    currentFrame++;

    if(currentFrame < NUM_SAMPLES){
        count = currentFrame;
    }else{
        count = NUM_SAMPLES;
    }

    ///Average all the frame times
    float frameTimeAverage = 0;
    for(int i = 0; i < count; i++){
        frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;

    if(frameTimeAverage > 0){
        _fps = 1000.0f / frameTimeAverage; // (ms/s)/(ms/frames) = fps
    }else{
        _fps = 60.0f;
    }

}

}
