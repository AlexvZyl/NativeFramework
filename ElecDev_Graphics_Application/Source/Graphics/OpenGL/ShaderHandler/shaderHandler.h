
// This unit does all of the shader hadling, including compiling and extracting the desired shader
// from the files.

#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// General.
#include <iostream>

// File streaming.
#include <iostream>
#include <fstream>
#include <sstream>

// OpenGL.
#include <glad/glad.h>
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Chaching include.
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------
//  Shader class.
//----------------------------------------------------------------------------------------------------------------------

class Shader
{
private:

    // Renderer ID.
    
    // Shader location.
    std::string m_filePath;
    // Caching uniform variables.
    std::unordered_map<std::string, int> m_uniformLocationCache;

public:

    unsigned int m_rendererID;

    // Constructor that generates shaders.
    Shader(std::string& filePath);
    // Destructor.
    ~Shader();

    // Bind and unbind the shader.
    void bind();
    void unbind();

    // Utility uniform functions.
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, glm::mat4 value);

private:

    // Used to get the shader matrix locations.
    int getUniformLocation(const std::string& name);

    // Function that compiles the shader program.
    void compileShader(const char* vShaderCode, const char* fShaderCode);

    // Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------