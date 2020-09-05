#ifndef CAPSULE_H
#define CAPSULE_H

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <Bengine/include/DebugRenderer.h>

class Capsule
{
    public:
        Capsule();
        virtual ~Capsule();

        void init(b2World* world,
                  const glm::vec2& position,
                  const glm::vec2& dimensions,
                  float density,
                  float friction,
                  bool fixedRotation = false);
        void drawDebug(Bengine::DebugRenderer& debugRenderer);

        b2Body* getBody() const { return m_body; }
        const glm::vec2& getDimensions() const { return m_dimensions; }

    protected:

    private:
        b2Body* m_body = nullptr;
        b2Fixture* m_fixtures[3];
        glm::vec2 m_dimensions;
};

#endif // CAPSULE_H
