#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <vector>
#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include "Vertex.h"

namespace Bengine{

enum class GlyphSortType{
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

struct Glyph{

    GLuint texture;
    float depth;

    Vertex topLeft;
    Vertex bottomLeft;
    Vertex topRight;
    Vertex bottomRight;

};

class RenderBatch{
public:
    RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
        offset(Offset),
        numVertices(NumVertices),
        texture(Texture){}
    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

class SpriteBatch{
public:
    SpriteBatch();
    virtual ~SpriteBatch();

    void init();

    void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
    void end();

    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);//Destination Rectangle: 2 pocition coordinates and 2 dimension coordinates

    void renderBatch();


private:
    void createRenderBatches();
    void createVertexArray();
    void sortGlyphs();

    static bool compareFrontToBack(Glyph* a, Glyph* b);
    static bool compareBackToFront(Glyph* a, Glyph* b);
    static bool compareTexture(Glyph* a, Glyph* b);

    GLuint _vbo;
    GLuint _vao;

    GlyphSortType _sortType;

    std::vector<Glyph*> _glyphs;
    std::vector<RenderBatch> _renderBatches;
};

}

#endif // SPRITEBATCH_H
