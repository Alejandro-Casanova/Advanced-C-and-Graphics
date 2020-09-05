#include "GLSLProgram.h"
#include "BengineErrors.h"

#include <fstream>
#include <vector>
#include "IOManager.h"

namespace Bengine{

GLSLProgram::GLSLProgram() : m_numAttributes(0),
                            m_programID(0),
                            m_vertexShaderID(0),
                            m_fragmentShaderID(0) {

}

GLSLProgram::~GLSLProgram(){
    //dtor
}

void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath){

    std::string vertSource;
    std::string fragSource;

    IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
    IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_programID = glCreateProgram();

    //Vertex shader object created
    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if(m_vertexShaderID == 0){
        fatalError("Vertex Shader failed to be created.");
    }

    //Vertex fragment object created
    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if(m_fragmentShaderID == 0){
        fatalError("Fragment Shader failed to be created");
    }

    ///Compile shaders
    compileShader(vertSource.c_str(), vertexShaderFilePath, m_vertexShaderID);
    compileShader(fragSource.c_str(), fragmentShaderFilePath, m_fragmentShaderID);


}

void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource){
    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_programID = glCreateProgram();

    //Vertex shader object created
    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if(m_vertexShaderID == 0){
        fatalError("Vertex Shader failed to be created.");
    }

    //Vertex fragment object created
    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if(m_fragmentShaderID == 0){
        fatalError("Fragment Shader failed to be created");
    }

    ///Compile shaders
    compileShader(vertexSource, "Vertex Shader", m_vertexShaderID);
    compileShader(fragmentSource, "Fragment Shader", m_fragmentShaderID);


}

void GLSLProgram::linkShaders(){

    // Attach our shaders to our program
    glAttachShader(m_programID, m_vertexShaderID);
    glAttachShader(m_programID, m_fragmentShaderID);

    // Link our program
    glLinkProgram(m_programID);

    //Error checking
    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);

    if (isLinked == GL_FALSE){

        GLint maxLength = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_programID, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_programID);
        // Don't leak shaders either.
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);

        // Use the infoLog as you see fit.
        printf("%s\n", &infoLog[0]);
        fatalError("Shaders failed to link.");
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_programID, m_vertexShaderID);
    glDetachShader(m_programID, m_fragmentShaderID);
}

//Add attribute to shader. Should be called between compiling and linking
void GLSLProgram::addAttribute(const std::string& attributeName){
    glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName){
    auto location = glGetUniformLocation(m_programID, uniformName.c_str());
    if(location == GL_INVALID_INDEX){
        fatalError("Uniform " + uniformName + " not found in shader!");
    }
    return location;
}

//Enable shader and all attributes
void GLSLProgram::use(){
    glUseProgram(m_programID);
    for(int i = 0; i < m_numAttributes; i++){
        glEnableVertexAttribArray(i);
    }
}

//Disable shader
void GLSLProgram::unuse(){
    glUseProgram(0);
    for(int i = 0; i < m_numAttributes; i++){
        glDisableVertexAttribArray(i);
    }
}

void GLSLProgram::dispose(){
    if(m_programID) glDeleteProgram(m_programID);
}

void GLSLProgram::compileShader(const char* source, const std::string& name, GLuint id){

    //Defines shader source, assigns source code read from file
    glShaderSource(id, 1, &source, nullptr);

    glCompileShader(id);

    //Error check
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if(success == GL_FALSE){
        //Saves log length into "maxLength"
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the null character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

        //Provide the infoLog in whatever manor you deem best
        //Exit with failure
        glDeleteShader(id);//Don't leak the shader

        printf("%s", &errorLog[0]);
        fatalError("Shader " + name + " failed to compile.");

    }
}
}
