#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // Include GLAD to get all the OpenGL headers that are needed
#include <glm/glm.hpp> //OpenGL Mathematics

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{

public:
    unsigned int ID;

    // The constructor reads and builds the shader from the given sources
    Shader(const char* vertexSource, const char* fragmentSource);
    // Use/Activate the shader
    void use();

    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat4(const std::string& name, float value1, float value2, float value3, float value4) const;
    



private :
    void checkComplieErrors(GLuint shader, std::string type);
};

#endif