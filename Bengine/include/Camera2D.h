#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Bengine{

//Camera class for 2D games
class Camera2D
{
    public:
        Camera2D();
        virtual ~Camera2D();

        //Sets orthographic matrix and screen dimensions
        void init(int screenWidth, int screenHeight);

        //Updates camera matrix if needed
        void update();

        glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

        bool isBoxInView(const glm::vec2& position, const glm::vec2& dimensions); /// < Checks if box is in camera view.

        //Setters
        void setPosition(const glm::vec2& newPosition){_position = newPosition; _needsMatrixUpdate = true;}
        void setScale(float newScale){_scale = newScale; _needsMatrixUpdate = true;}

        //Getters
        glm::vec2 getPosition(){return _position;}
        float getScale(){return _scale;}
        glm::mat4 getCameraMatrix(){return _cameraMatrix;}


    private:
        int _screenWidth, _screenHeight;
        bool _needsMatrixUpdate;
        float _scale;
        glm::vec2 _position; //Centered
        glm::mat4 _cameraMatrix;
        glm::mat4 _orthoMatrix;
};
}

#endif // CAMERA2D_H
