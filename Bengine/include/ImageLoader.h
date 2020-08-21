#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>

#include "GLTexture.h"

namespace Bengine{

    class ImageLoader
    {
        public:
            static GLTexture loadPNG(std::string filePath);
        private:
    };

}

#endif // IMAGELOADER_H
