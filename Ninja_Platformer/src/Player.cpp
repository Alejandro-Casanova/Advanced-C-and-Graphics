#include "Player.h"
#include <Bengine/include/ResourceManager.h>
#include <SDL2/SDL.h>
#include <iostream>

Player::Player()
{
    //ctor
}

Player::~Player()
{
    //dtor
}

void Player::init(b2World* world, const glm::vec2& position, const glm::vec2& drawDims, const glm::vec2& collitionDims, const Bengine::ColorRGBA8& color){
    Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Assets/blue_ninja.png");
    m_color = color;
    m_drawDims = drawDims;
    m_capsule.init(world, position, collitionDims, 1.0f, 0.1f, true);
    m_texture.init(texture, glm::ivec2(10, 2));

}

void Player::draw(Bengine::SpriteBatch& spriteBatch){
    glm::vec4 destRect;
    b2Body* body = m_capsule.getBody();
    destRect.x = body->GetPosition().x - m_drawDims.x / 2.0f;
    destRect.y = body->GetPosition().y - m_capsule.getDimensions().y / 2.0f;
    destRect.z = m_drawDims.x;
    destRect.w = m_drawDims.y;

    int tileIndex;
    int numTiles;
    float animSpeed = 0.2f;

    glm::vec2 velocity;
    velocity.x = body->GetLinearVelocity().x;
    velocity.y = body->GetLinearVelocity().y;

    ///Calculate animation
    if(m_onGround){
        if(m_isPunching){
            ///Punching
            numTiles = 4;
            tileIndex = 1;
            if(m_moveState != PlayerMoveState::PUNCHING){///< Restart running animation
                m_moveState = PlayerMoveState::PUNCHING;
                m_animTime = 0;
            }


        }else if( (abs(velocity.x) < 1.0f) || ( (velocity.x * m_direction) < 0 ) ){
            ///Standing
            m_moveState = PlayerMoveState::STANDING;
            numTiles = 1;
            tileIndex = 0;
            animSpeed = abs(velocity.x) * 0.025f;
        }else{
            ///Running
            numTiles = 6;
            tileIndex = 10;
            if(m_moveState != PlayerMoveState::RUNNING){///< Restart running animation
                m_moveState = PlayerMoveState::RUNNING;
                m_animTime = 0;
            }
        }
    }else{
        ///In the air
        if(m_isPunching){
            ///Kick
            numTiles = 1;
            tileIndex = 18;
            animSpeed *= 0.25f; ///< Plays animation slower, as it only has 1 frame
            if(m_moveState != PlayerMoveState::PUNCHING){///< Restart running animation
                m_moveState = PlayerMoveState::PUNCHING;
                m_animTime = 0;
            }
        }else if(velocity.y <= 0){///< Falling
            m_moveState = PlayerMoveState::IN_AIR;
            numTiles = 1;
            tileIndex = 17;
        }else{///< Rising
            m_moveState = PlayerMoveState::IN_AIR;
            numTiles = 1;
            tileIndex = 16;
        }
    }

    ///Increment animation time
    m_animTime += animSpeed;

    ///Check for punch end
    if( (m_moveState == PlayerMoveState::PUNCHING) && (m_animTime > numTiles) ){
        m_isPunching = false;
    }

    ///Calculate animation tile
    tileIndex += (int)m_animTime % numTiles;

    ///Get UV coordinates from tile index
    glm::vec4 uvRect = m_texture.getUVs(tileIndex);

    ///Check direction
    if(m_direction == -1){
        ///Flip texture horizontally
        uvRect.x += 1.0f / m_texture.dims.x;
        uvRect.z *= -1;
    }

    spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, m_color, body->GetAngle());
}

void Player::drawDebug(Bengine::DebugRenderer& debugRenderer){
    m_capsule.drawDebug(debugRenderer);
}

void Player::update(Bengine::InputManager& inputManager){
    b2Body* body = m_capsule.getBody();
    if(inputManager.isKeyDown(SDLK_a)){
        body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
        m_direction = -1;
    }else if(inputManager.isKeyDown(SDLK_d)){
        body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
        m_direction = 1;
    }
    ///Apply damping
    else{
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95, body->GetLinearVelocity().y));
    }

    ///Check for punch
    if(inputManager.isKeyPressed(SDLK_SPACE)) m_isPunching = true;

    const float MAX_SPEED = 10.0f;
    if(body->GetLinearVelocity().x < -MAX_SPEED){
        body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
    }else if(body->GetLinearVelocity().x > MAX_SPEED){
        body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
    }

    ///Loop through all the player's contact points
    m_onGround = false;
    for(b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next){
        b2Contact* c = ce->contact;
        if(c->IsTouching()){
            b2WorldManifold manifold;
            c->GetWorldManifold(&manifold);
            ///Check if the points are below
            //bool below = false;
            for(int i = 0; i < b2_maxManifoldPoints; i++){
                if(manifold.points[i].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + m_capsule.getDimensions().x / 2.0f + 0.1f){
                    //below = true;
                    m_onGround = true;
                    break;
                }
            }
            if(m_onGround){
                ///We can jump
                if(inputManager.isKeyPressed(SDLK_w)){
                    body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 40.0f), true);
                    break;
                }
            }
        }
    }

}
