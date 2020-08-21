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

        void pressKey(unsigned int keyID);
        void releaseKey(unsigned int keyID);

        void setMouseCoords(float x, float y);

        bool isKeyPressed(unsigned int keyID) const;

        glm::vec2 getMouseCoords() const {return _mouseCoords;}

    private:
        std::unordered_map<unsigned int, bool> _keyMap;
        glm::vec2 _mouseCoords;

};

}

#endif // INPUTMANAGER_H
