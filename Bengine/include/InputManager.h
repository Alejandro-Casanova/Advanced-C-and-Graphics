#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <glm/glm.hpp>

namespace Bengine{

class InputManager
{
    public:
        InputManager();
        virtual ~InputManager();

        void update();

        void pressKey(unsigned int keyID);
        void releaseKey(unsigned int keyID);

        void setMouseCoords(float x, float y);

        bool isKeyDown(unsigned int keyID) const;
        bool isKeyPressed(unsigned int keyID) const;

        glm::vec2 getMouseCoords() const {return _mouseCoords;}

    private:
        bool wasKeyDown(unsigned int keyID) const;

        std::unordered_map<unsigned int, bool> _keyMap;
        std::unordered_map<unsigned int, bool> _prevKeyMap;
        glm::vec2 _mouseCoords;

};

}

#endif // INPUTMANAGER_H
