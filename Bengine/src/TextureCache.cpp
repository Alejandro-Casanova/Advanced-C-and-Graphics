#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace Bengine{

    TextureCache::TextureCache()
    {
        //ctor
    }

    TextureCache::~TextureCache()
    {
        //dtor
    }

    GLTexture TextureCache::getTexture(std::string texturePath){

        //look up texture
        auto mit = _textureMap.find(texturePath);
        //Check if not found in the map
        if(mit == _textureMap.end()){
            //Load texture from file
            GLTexture newTexture = ImageLoader::loadPNG(texturePath);
            //Save new texture as "pair" and insert it in the map
            _textureMap.insert(make_pair(texturePath, newTexture));

            std::cout << "Loaded texture!\n";
            return newTexture;
        }
        std::cout << "Used cached texture!\n";
        return mit->second;

    }
}
