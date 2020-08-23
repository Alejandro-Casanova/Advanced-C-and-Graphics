#ifndef SPRITE_H
#define SPRITE_H

#include "GLTexture.h"

#include <GLEW/glew.h>
#include <string>

namespace Bengine{

class Sprite
{
    public:
        Sprite();
        virtual ~Sprite();

        void draw();
        void init(float x, float y, float width, float height, std::string texturePath);

    private:
        float _x;
        float _y;
        float _width;
        float _height;
        GLuint _vboID;
        GLTexture _texture;
};

}

#endif // SPRITE_H
