#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // Include GLAD to get all the OpenGL headers that are needed

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{

public:
    unsigned int ID;

    // The constructor reads and builds the shader from the given sources
    Shader(const char* vertexSource, const char* fragmentSource)
    {
        // Step 1: Retrive and read the vertex/fragment source codes
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // Open the files
            vShaderFile.open(vertexSource);
            fShaderFile.open(fragmentSource);
            std::stringstream vShaderStream, fShaderStream;
            //Read the file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            //Convert read data into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER.H - FILE NOT SUCCESFULLY READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        // Step 2: Compile the shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print any compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER.H - VERTEX COMPILATION FAILED\n" << infoLog << std::endl;
        };

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Print any compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER.H - FRAGMENT COMPILATION FAILED\n" << infoLog << std::endl;
        };
        

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        //Print any linking errors
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER.H - PROGRAM LINKING FAILED\n" << infoLog << std::endl;
        }
        
        // Delete shaders as they're linked into our program now and are no longer needed
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }

    // Use/Activate the shader
    void use()
    {
        glUseProgram(ID);
    }
    // Utility uniform functions
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat4(const std::string &name, float value1, float value2, float value3, float value4 ) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
    }
};

#endif