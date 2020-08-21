#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <string>
#include <GLEW/glew.h>
namespace Bengine{
    class GLSLProgram
    {
        public:
            GLSLProgram();
            virtual ~GLSLProgram();

            void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

            void linkShaders();

            void addAttribute(const std::string& attributeName);

            GLint getUniformLocation(const std::string& uniformName);

            void use();
            void unuse();

        private:
            int _numAttributes;

            void compileShader(const std::string& filePath, GLuint id);

            GLuint _programID;
            GLuint _vertexShaderID;
            GLuint _fragmentShaderID;
    };
}

#endif // GLSLPROGRAM_H
