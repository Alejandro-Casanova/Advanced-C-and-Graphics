#include "IMainGame.h"
#include "Timing.h"


#include "IGameScreen.h"

namespace Bengine{

IMainGame::IMainGame(){
    m_screenList = std::make_unique<ScreenList>(this);
}

IMainGame::~IMainGame(){}

void IMainGame::run(){

    if(!init()) return;

    FpsLimiter limiter;
    limiter.init(m_maxFps);

    ///TimeStep parameters
    const float DESIRED_fps = 60.0f;
    const int MAX_PHYSICS_STEPS = 4; //Max steps simulated in one frame. Prevents "spiral of death", where too many steps prevent the program from rendering
    const float MS_PER_SECOND = 1000.0f;
    const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIRED_fps;
    const float MAX_DELTA_TIME = 1.0f;

    float prevTicks = SDL_GetTicks();

    ///Game loop
    m_isRunning = true;
    while(m_isRunning){
        limiter.begin();

        ///Delta Time Calculation
        Uint32 newTicks = SDL_GetTicks();
        Uint32 frameTime = newTicks - prevTicks;
        prevTicks = newTicks;
        float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;
        m_totalDeltaTime = totalDeltaTime;

        int i = 0;//Prevents "spiral of death"
        while( ( totalDeltaTime > 0.0f ) && ( i < MAX_PHYSICS_STEPS ) ){
            float deltaTime = std::min(MAX_DELTA_TIME, totalDeltaTime);

            ///Call custom update and draw methods
            inputManager.update();

            update(deltaTime);

            i++;
            totalDeltaTime -= deltaTime;
        }

        if(m_isRunning){///< Prevents crash
            draw();
            m_window.swapBuffer();
        }

    m_fps = limiter.end();
    }
}

void IMainGame::exitGame(){
    m_currentScreen->onExit();
    if(m_screenList){
        m_screenList->destroy();
        m_screenList.reset();
    }
    m_isRunning = false;
}

void IMainGame::update(float deltaTime){
    if(m_currentScreen){///< Checks m_currentScreen != nullptr
        switch(m_currentScreen->getScreenState()){
        case ScreenState::RUNNING:
            m_currentScreen->update(deltaTime);
            break;
        case ScreenState::CHANGE_NEXT:
            m_currentScreen->onEntry();
            m_currentScreen = m_screenList->moveNext();
            if(m_currentScreen == nullptr){
                m_currentScreen->setRunning();
                m_currentScreen->onEntry();
            }
            break;
        case ScreenState::CHANGE_PREVIOUS:
            m_currentScreen->onEntry();
            m_currentScreen = m_screenList->movePrevious();
            if(m_currentScreen == nullptr){
                m_currentScreen->setRunning();
                m_currentScreen->onEntry();
            }
            break;
        case ScreenState::EXIT_APPLICATION:
            exitGame();
            break;
        default:
            break;
        }
    }else{
        exitGame();
    }
}

void IMainGame::draw(){
    glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
    if(m_currentScreen && (m_currentScreen->getScreenState() == ScreenState::RUNNING)){
        m_currentScreen->draw();
    }
}

void IMainGame::onSDLEvent(SDL_Event& evnt){
    switch(evnt.type){
        case SDL_QUIT:
            exitGame();
            break;
        case SDL_MOUSEMOTION:
            inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
            break;
        case SDL_KEYDOWN:
            inputManager.pressKey(evnt.key.keysym.sym);
            break;
        case SDL_KEYUP:
            inputManager.releaseKey(evnt.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            inputManager.pressKey(evnt.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            inputManager.releaseKey(evnt.button.button);
            break;
    }
}

bool IMainGame::init(){
    Bengine::init();

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    if(!initSystems()) return false;

    onInit();
    addScreens();

    m_currentScreen = m_screenList->getCurrent();
    m_currentScreen->onEntry();
    m_currentScreen->setRunning();

    return true;
}

bool IMainGame::initSystems(){
    if(m_window.create("Default", m_screenWidth, m_screenHeight, 0) != 0) return false;
    return true;
}

}
