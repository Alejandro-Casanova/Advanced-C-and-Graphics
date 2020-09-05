#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

class Light{
public:

    void draw(Bengine::SpriteBatch& spriteBatch){
        glm::vec4 destRect;
        ///Spritebatch draw function needs the origin of the bodies to be on their corner (not centered)
        destRect.x = position.x - size / 2.0f;
        destRect.y = position.y - size / 2.0f;
        destRect.z = size;
        destRect.w = size;

        ///Draw light
        spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, 0.0f);
    }

    Bengine::ColorRGBA8 color;
    glm::vec2 position;
    float size;
};


#endif // LIGHT_H_INCLUDED
