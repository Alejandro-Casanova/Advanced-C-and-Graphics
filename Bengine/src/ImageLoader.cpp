#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "BengineErrors.h"

#include <vector>

namespace Bengine{

    GLTexture ImageLoader::loadPNG(std::string filePath){

        GLTexture texture = {}; //Initialize struct to 0

        std::vector<unsigned char> in;
        std::vector<unsigned char> out;
        unsigned long width, height;

        //Error check and file read into buffer
        if(IOManager::readFileToBuffer(filePath, in) == false){
            fatalError("Failed to load PNG into buffer.");
        }

        //PNG decoded and error check
        int errorCode = decodePNG(out, width, height, in.data(), in.size());
        if(errorCode != 0){
            fatalError("decodePNG failed with error: " + std::to_string(errorCode));
        }

        //Generate texture object and save id
        glGenTextures(1, &(texture.id));
        //Bind texture object
        glBindTexture(GL_TEXTURE_2D, texture.id);
        //Define texture image, save data into texture object
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, out.data());

        //Define texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        //Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        //Save wifth and height into texture object
        texture.width = width;
        texture.height = height;

        return texture;
    }
}
