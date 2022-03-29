
// This unit does all of the shader hadling, including compiling and extracting the desired shader
// from the files.

#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Chaching include.
#include <unordered_map>
#include "GLM/glm.hpp"

//=============================================================================================================================================//
//  Class.  																																   //
//=============================================================================================================================================//

class Shader
{
public:

    // Shader renderer ID.
    unsigned int m_rendererID;

    // Contructors.
    Shader(unsigned int shaderID);
    // Destructor.
    ~Shader();

    // Bind the shader.
    void bind();
    // Unbind the shader.
    void unbind();

    // Set a boolean variable in the shader.
    void setBool(const std::string& name, bool value);
    // Set a int variable in the shader.
    void setInt(const std::string& name, int value);
    // Set an int array in the shader.
    void setIntArray(const std::string& name, const int* value, unsigned count);
    // Set a float variable in the shader.
    void setFloat(const std::string& name, float value);
    // Set a mat4 variable in the shader.
    void setMat4(const std::string& name, glm::mat4* value);
    // Set a multisampled sampler vaue.
    void setSamplerMSAA(const std::string& name, int* textureID, unsigned count);

private:

    // Caching uniform variables.
    std::unordered_map<std::string, int> m_uniformLocationCache;

    // Used to get the shader matrix locations.
    int getUniformLocation(const std::string& name);
    // Function that compiles the shader program.
    void compileShader(std::string& vShaderCode, std::string& gShaderCode, std::string& fShaderCode);
    // Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);

};

//=============================================================================================================================================//
//  EOF.    																																   //
//=============================================================================================================================================//