
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

//----------------------------------------------------------------------------------------------------------------------
//  Shader class.
//----------------------------------------------------------------------------------------------------------------------

class Shader
{
public:

    unsigned int ID;

    // Constructor that generates shaders.
    Shader(std::string& filePath);
    // Empty constructor.
    Shader();

    // Use the shader.
    void use();

    // Utility uniform functions.
    void setBool(const std::string& name, bool value) const;
  
    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setMat4(const std::string& name, glm::mat4 value) const;

private:

    // Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------