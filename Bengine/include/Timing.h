#ifndef TIMING_H_INCLUDED
#define TIMING_H_INCLUDED

namespace Bengine{

class FpsLimiter{

public:
    FpsLimiter();
    void init(float maxFPS);
    void setMaxFPS(float maxFPS);
    void begin();
    float end(); //Will return FPS value

private:
    void calculateFps();

    float _fps;
    float _maxFps;
    float _frameTime;
    unsigned int _startTicks;
};

}

#endif // TIMING_H_INCLUDED
