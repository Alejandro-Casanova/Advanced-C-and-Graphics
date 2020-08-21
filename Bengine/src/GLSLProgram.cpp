#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>
#include <vector>

namespace Bengine{

    GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0) {

    }

    GLSLProgram::~GLSLProgram(){
        //dtor
    }

    void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath){

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        _programID = glCreateProgram();

        //Vertex shader object created
        _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if(_vertexShaderID == 0){
            fatalError("Vertex Shader failed to be created.");
        }

        //Vertex fragment object created
        _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if(_fragmentShaderID == 0){
            fatalError("Fragment Shader failed to be created");
        }

        compileShader(vertexShaderFilePath, _vertexShaderID);
        compileShader(fragmentShaderFilePath, _fragmentShaderID);


    }

    void GLSLProgram::linkShaders(){

        // Attach our shaders to our program
        glAttachShader(_programID, _vertexShaderID);
        glAttachShader(_programID, _fragmentShaderID);

        // Link our program
        glLinkProgram(_programID);

        //Error checking
        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);

        if (isLinked == GL_FALSE){

            GLint maxLength = 0;
            glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(_programID);
            // Don't leak shaders either.
            glDeleteShader(_vertexShaderID);
            glDeleteShader(_fragmentShaderID);

            // Use the infoLog as you see fit.
            printf("%s\n", &infoLog[0]);
            fatalError("Shaders failed to link.");
        }

        // Always detach shaders after a successful link.
        glDetachShader(_programID, _vertexShaderID);
        glDetachShader(_programID, _fragmentShaderID);
    }

    //Add attribute to shader. Should be called between compiling and linking
    void GLSLProgram::addAttribute(const std::string& attributeName){
        glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
    }

    GLint GLSLProgram::getUniformLocation(const std::string& uniformName){
        GLint location = glGetUniformLocation(_programID, uniformName.c_str());
        if(location == GL_INVALID_INDEX){
            fatalError("Uniform " + uniformName + " not found in shader!");
        }
        return location;
    }

    //Enable shader and all attributes
    void GLSLProgram::use(){
        glUseProgram(_programID);
        for(int i = 0; i < _numAttributes; i++){
            glEnableVertexAttribArray(i);
        }
    }

    //Disable shader
    void GLSLProgram::unuse(){
        glUseProgram(0);
        for(int i = 0; i < _numAttributes; i++){
            glDisableVertexAttribArray(i);
        }
    }

    void GLSLProgram::compileShader(const std::string& filePath, GLuint id){

        //Vertex file opened
        std::ifstream vertexFile(filePath);
        if(vertexFile.fail()){
            perror(filePath.c_str());
            fatalError("Failed to open Vertex File: " + filePath);
        }

        //Vertex File read
        std::string fileContents = "";
        std::string line;

        while(std::getline(vertexFile, line)){
            fileContents += line + "\n";
        }

        vertexFile.close();

        //Defines shader source, assigns source code read from file
        const char* contentsPtr = fileContents.c_str();
        glShaderSource(id, 1, &contentsPtr, nullptr);

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

            printf("%s\n", &errorLog[0]);
            fatalError("Shader " + filePath + "Failed to compile.");

        }
    }
}
