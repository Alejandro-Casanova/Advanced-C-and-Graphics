#ifndef BOX_H
#define BOX_H

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <Bengine/include/Vertex.h>
#include <Bengine/include/GLTexture.h>
#include <Bengine/include/SpriteBatch.h>

class Box
{
    public:
        Box();
        virtual ~Box();

        void init(b2World* world,
                  const glm::vec2& position,
                  const glm::vec2& dimensions,
                  Bengine::GLTexture texture,
                  const Bengine::ColorRGBA8& color,
                  bool fixedRotation = false,
                  glm::vec4 uvRectangle = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        void draw(Bengine::SpriteBatch& spriteBatch);

        b2Body* getBody() const { return m_body; }
        b2Fixture* getFixture() const { return m_fixture; }
        const glm::vec2& getDimensions() const { return m_dimensions; }
        const Bengine::ColorRGBA8& getColor() const { return m_color; }

    protected:

    private:
        glm::vec4 m_uvRect;
        b2Body* m_body = nullptr;
        b2Fixture* m_fixture = nullptr;
        glm::vec2 m_dimensions;
        Bengine::ColorRGBA8 m_color;
        Bengine::GLTexture m_texture;
};

#endif // BOX_H
