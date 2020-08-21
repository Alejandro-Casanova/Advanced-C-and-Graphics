#include "Timing.h"

#include <SDL2/SDL.h>

namespace Bengine{

FpsLimiter::FpsLimiter(){
}

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

    float frameTicks = SDL_GetTicks() - _startTicks;
        //Limits the FPS to the max FPS
        if(1000.0f / _maxFps > frameTicks){
            SDL_Delay(1000.0f / _maxFps - frameTicks);
        }
    return _fps;
}

void FpsLimiter::calculateFps(){

    static const int NUM_SAMPLES = 10;
    static float frameTimes[NUM_SAMPLES];
    static int currentFrame = 0;

    static float prevTicks = SDL_GetTicks();

    float currentTicks = SDL_GetTicks();

    _frameTime = currentTicks - prevTicks;
    frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

    prevTicks = currentTicks;

    int count;
    currentFrame++;

    if(currentFrame < NUM_SAMPLES){
        count = currentFrame;
    }else{
        count = NUM_SAMPLES;
    }

    float frameTimeAverage = 0;
    for(int i = 0; i < count; i++){
        frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;

    if(frameTimeAverage > 0){
        _fps = 1000.0f / frameTimeAverage; // (ms/s)/(ms/frames) = fps
    }else{
        _fps = 0.0f;
    }

}

}
