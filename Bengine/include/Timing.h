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

    float _fps = 0.0f;
    float _maxFps = 0.0f;
    float _frameTime = 0.0f;
    unsigned int _startTicks = 0;
};

}

#endif // TIMING_H_INCLUDED
