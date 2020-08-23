#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <map>
#include <string>

#include "GLTexture.h"

namespace Bengine{

class TextureCache
{
    public:
        TextureCache();
        virtual ~TextureCache();

        GLTexture getTexture(std::string texturePath);

    private:
        std::map<std::string, GLTexture> _textureMap;
};

}

#endif // TEXTURECACHE_H
