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
        void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

        void linkShaders();

        void addAttribute(const std::string& attributeName);

        GLint getUniformLocation(const std::string& uniformName);

        void use();
        void unuse();

        void dispose();

    private:
        int m_numAttributes;

        void compileShader(const char* source, const std::string& name, GLuint id);

        GLuint m_programID;
        GLuint m_vertexShaderID;
        GLuint m_fragmentShaderID;
};

}

#endif // GLSLPROGRAM_H
