#include "IOManager.h"

#include <fstream>

namespace Bengine{

    bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer){
        std::ifstream file(filePath, std::ios::binary);
        if(file.fail()){
            perror(filePath.c_str());
            return false;
        }

        //Seek to the end of the file
        file.seekg(0, std::ios::end);

        //Get the file size
        int fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        //Disregard size of file header, posible data befor file content
        fileSize -= file.tellg();

        buffer.resize(fileSize);
        file.read((char*)buffer.data(), fileSize);
        file.close();

        return true;
    }
}
