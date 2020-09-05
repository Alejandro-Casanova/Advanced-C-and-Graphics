#include "Capsule.h"

Capsule::Capsule()
{
    //ctor
}

Capsule::~Capsule()
{
    //dtor
}

void Capsule::init(b2World* world,
                  const glm::vec2& position,
                  const glm::vec2& dimensions,
                  float density,
                  float friction,
                  bool fixedRotation /*= false*/){

    m_dimensions = dimensions;
    ///Make the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = fixedRotation;
    bodyDef.position.Set(position.x, position.y);
    m_body = world->CreateBody(&bodyDef);

    ///Make shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox(dimensions.x / 2.0f - 0.01f, (dimensions.y - dimensions.x) / 2.0f);///< Correction applied to x dimension to prevent box corner from getting stuck on ledges

    ///Make fixture (BOX)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    m_fixtures[0] = m_body->CreateFixture(&fixtureDef);

    ///Make circles
    b2CircleShape circleShape;
    circleShape.m_radius = dimensions.x / 2.0f;

    b2FixtureDef circleDef;
    circleDef.shape = &circleShape;
    circleDef.density = density;
    circleDef.friction = friction;

    ///Bottom Circle
    circleShape.m_p.Set(0.0f, ( -m_dimensions.y + m_dimensions.x ) / 2.0f);
    m_fixtures[1] = m_body->CreateFixture(&circleDef);

    ///Top Circle
    circleShape.m_p.Set(0.0f, ( m_dimensions.y + -m_dimensions.x ) / 2.0f);
    m_fixtures[2] = m_body->CreateFixture(&circleDef);
}

void Capsule::drawDebug(Bengine::DebugRenderer& debugRenderer){
    ///Draw box
    glm::vec4 destRect;
    destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
    destRect.y = m_body->GetPosition().y - ( m_dimensions.y - m_dimensions.x ) / 2.0f;
    destRect.z = m_dimensions.x;
    destRect.w = m_dimensions.y - m_dimensions.x;
    debugRenderer.drawBox(destRect, Bengine::ColorRGBA8(255, 255, 255, 255), m_body->GetAngle());

    ///Draw Circles
    debugRenderer.drawCircle(glm::vec2(destRect.x + m_dimensions.x / 2.0f, destRect.y),
                             Bengine::ColorRGBA8(255, 255, 255, 255), m_dimensions.x / 2.0f);

    debugRenderer.drawCircle(glm::vec2(destRect.x + m_dimensions.x / 2.0f, destRect.y + destRect.w),
                             Bengine::ColorRGBA8(255, 255, 255, 255), m_dimensions.x / 2.0f);

}
