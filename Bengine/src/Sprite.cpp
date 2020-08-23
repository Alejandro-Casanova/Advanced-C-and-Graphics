#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace Bengine{

Sprite::Sprite() : _x(0.0f), _y(0.0f), _width(0.0f), _height(0.0f), _vboID(0)
{

}

Sprite::~Sprite(){
    if(_vboID != 0){
        glDeleteBuffers(1, &_vboID);
    }
}

void Sprite::init(float x, float y, float width, float height, std::string texturePath){
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    _texture = ResourceManager::getTexture(texturePath);

    //Generate vertex buffer object (if not generated already)
    if(_vboID == 0){
        glGenBuffers(1, &_vboID);
    }

    Vertex vertexData[6];

    //First triangle
    vertexData[0].setPosition(x + width, y + height);
    vertexData[0].setUV(1.0f, 1.0f);

    vertexData[1].setPosition(x, y + height);
    vertexData[1].setUV(0.0f, 1.0f);

    vertexData[2].setPosition(x, y);
    vertexData[2].setUV(0.0f, 0.0f);

    //First triangle
    vertexData[3].setPosition(x, y);
    vertexData[3].setUV(0.0f, 0.0f);

    vertexData[4].setPosition(x + width, y);
    vertexData[4].setUV(1.0f, 0.0f);

    vertexData[5].setPosition(x + width, y + height);
    vertexData[5].setUV(1.0f, 1.0f);

    for(int i = 0; i < 6; i++){
        vertexData[i].setColor(0, 255, 255, 255);
    }

    vertexData[1].setColor(255, 0, 0, 255);
    vertexData[4].setColor(0, 0, 255, 255);

    //Tell opengl to bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    //Upload data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    //Unbind buffer (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw(){

    glBindTexture(GL_TEXTURE_2D, _texture.id);
    //Indicate active buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);




    //Draws triangles from 6 vertexes
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //Disable first array (_vboID only contains one)
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    //Unbind-Deactivate current buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
