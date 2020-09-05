#ifndef PLAYER_H
#define PLAYER_H

#include "Capsule.h"
#include <Bengine/include/SpriteBatch.h>
#include <Bengine/include/TileSheet.h>
#include <Bengine/include/InputManager.h>

enum class PlayerMoveState{ STANDING, RUNNING, PUNCHING, IN_AIR };

class Player
{
    public:
        Player();
        virtual ~Player();

        void init(b2World* world,
                  const glm::vec2& position,
                  const glm::vec2& drawDims,
                  const glm::vec2& collitionDims,
                  const Bengine::ColorRGBA8& color);

        void draw(Bengine::SpriteBatch& spriteBatch);
        void drawDebug(Bengine::DebugRenderer& debugRenderer);

        void update(Bengine::InputManager& inputManager);

        const Capsule& getCapsule() const { return m_capsule; }
        glm::vec2 getPosition() const { return glm::vec2(m_capsule.getBody()->GetPosition().x, m_capsule.getBody()->GetPosition().y); }

    protected:

    private:
        Capsule m_capsule;
        Bengine::TileSheet m_texture;
        Bengine::ColorRGBA8 m_color;
        glm::vec2 m_drawDims;
        PlayerMoveState m_moveState = PlayerMoveState::STANDING;
        int m_direction = 1; ///< 1 or -1
        bool m_onGround = false;
        bool m_isPunching = false;
        float m_animTime = 0.0f; ///< Used for animation
};

#endif // PLAYER_H
