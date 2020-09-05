#ifndef TILESHEET_H_INCLUDED
#define TILESHEET_H_INCLUDED

#include "GLTexture.h"

namespace Bengine{

class TileSheet{
public:
    void init(const GLTexture& texture, const glm::ivec2& tileDims){
        this->texture = texture;
        dims = tileDims;
    }

    ///SpriteSheet indexes start at bottom left. (Tile (0,0) is bottom left corner)
    glm::vec4 getUVs(int index){
        int xTile = index % dims.x;
        int yTile = index / dims.x; ///< Integer division

        glm::vec4 uvs;
        uvs.x = xTile / (float)dims.x;
        uvs.y = yTile / (float)dims.y;
        uvs.z = 1.0f / dims.x;
        uvs.w = 1.0f / dims.y;

        return uvs;
    }

    GLTexture texture;
    glm::ivec2 dims;
};

}


#endif // TILESHEET_H_INCLUDED
