#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>
#include "GLSLProgram.h"

namespace Bengine{

class DebugRenderer
{
    public:
        DebugRenderer();
        virtual ~DebugRenderer();

        void init();
        void end();
        void drawBox(const glm::vec4& destRect, const Bengine::ColorRGBA8& color, float angle);
        void drawCircle(const glm::vec2& center, const Bengine::ColorRGBA8& color, float radius);
        void render(const glm::mat4& projectionMatrix, float lineWidth);
        void dispose();

        struct DebugVertex{
            glm::vec2 position;
            Bengine::ColorRGBA8 color;
        };

    private:
        Bengine::GLSLProgram m_program;
        std::vector<DebugVertex> m_verts;
        std::vector<GLuint> m_indices;
        GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
        int m_numElements = 0;
};

}

#endif // DEBUGRENDERER_H
