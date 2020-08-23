#ifndef GLTEXTURE_H_INCLUDED
#define GLTEXTURE_H_INCLUDED

#include <GLEW/glew.h>

namespace Bengine{
    struct GLTexture{
        GLTexture() : id(0), height(0), width(0){}
        GLuint id;
        int height;
        int width;
    };
}

#endif // GLTEXTURE_H_INCLUDED
