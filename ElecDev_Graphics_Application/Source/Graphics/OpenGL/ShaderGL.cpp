
// This unit does all of the shader hadling, including compiling and extracting the desired shader
// from the files.

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "../Resources/ResourceHandler.h"
#include "OpenGL/ShaderGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "External/Misc/ConsoleColor.h"
#include "Utilities/Logger/Logger.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

Shader::Shader(unsigned int shaderID)
    : m_rendererID(0)
{
    // Shader mode handler. Used as an index for the shaderSource.
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };
    ShaderType shaderType = ShaderType::NONE;

    // Parse the shader.
    //----------------------------------------------------------------------------

    // Stream that contains the shader.
    std::stringstream shaderSource[3];
    std::istringstream source(loadTextFromResource(shaderID));

    // Run through all of the lines.
    std::string line;
    while (std::getline(source, line))
    {
        // Find shader keyword and set shader type.
        if (line.find("#shader") != std::string::npos)
        {
            // Check for vertex shader.
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::VERTEX;
            // Check for fragment shader.
            else if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::FRAGMENT;
            else if (line.find("geometry") != std::string::npos)
                shaderType = ShaderType::GEOMETRY;
        }

        // Any other line of code.
        else
        {
            shaderSource[(int)shaderType] << line << "\n";
        }
    }

    // Store shader source in correct format.
    std::string vShaderString = shaderSource[0].str();
    std::string fShaderString = shaderSource[1].str();
    std::string gShaderString = shaderSource[2].str();

    //----------------------------------------------------------------------------

    // Compile the shader program.
    compileShader(vShaderString, gShaderString, fShaderString); 
}

// Destructor
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererID));
}

// Compiles the shader program.
void Shader::compileShader(std::string& vShaderCode, std::string& gShaderCode, std::string& fShaderCode)
{
    // Shader ID's.
    unsigned int vertex, fragment, geometry;

    // Create shader program.
    GLCall(m_rendererID = glCreateProgram());

    // Vertex shader.
    if (vShaderCode.size())
    {
        const char* vShaderSource = vShaderCode.c_str();
        GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
        GLCall(glShaderSource(vertex, 1, &vShaderSource, NULL));
        GLCall(glCompileShader(vertex));
        checkCompileErrors(vertex, "Vertex");
        GLCall(glAttachShader(m_rendererID, vertex));
        GLCall(glDeleteShader(vertex));
    }

    // Vertex shader.
    if (gShaderCode.size())
    {
        const char* gShaderSource = gShaderCode.c_str();
        GLCall(geometry = glCreateShader(GL_GEOMETRY_SHADER));
        GLCall(glShaderSource(geometry, 1, &gShaderSource, NULL));
        GLCall(glCompileShader(geometry));
        checkCompileErrors(geometry, "Geometry");
        GLCall(glAttachShader(m_rendererID, geometry));
        GLCall(glDeleteShader(geometry));
    }

    // Fragment Shader.
    if (fShaderCode.size())
    {
        const char* fShaderSource = fShaderCode.c_str();
        GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
        GLCall(glShaderSource(fragment, 1, &fShaderSource, NULL));
        GLCall(glCompileShader(fragment));
        checkCompileErrors(fragment, "Fragment");
        GLCall(glAttachShader(m_rendererID, fragment));
        GLCall(glDeleteShader(fragment));
    }

    // Link shader.
    GLCall(glLinkProgram(m_rendererID));
    checkCompileErrors(m_rendererID, "Program");
}

void Shader::bind()
{
    GLCall( glUseProgram( m_rendererID ) );
}

void Shader::unbind()
{
    GLCall(glUseProgram(0));
}

void Shader::setBool(const std::string& name, bool value)
{
    GLCall( glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), (int)value) );
}
void Shader::setInt(const std::string& name, int value)
{
    GLCall( glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), value) );
}
void Shader::setIntArray(const std::string& name, const int* value, unsigned count)
{
    GLCall(glUniform1iv(glGetUniformLocation(m_rendererID, name.c_str()), count, value));
}
void Shader::setFloat(const std::string& name, float value)
{
    GLCall( glUniform1f(glGetUniformLocation(m_rendererID, name.c_str()), value) );
}
void Shader::setMat4(const std::string& name, glm::mat4* value)
{
    GLCall ( glUniformMatrix4fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, (const float*)value ) );
}
void Shader::setSamplerMSAA(const std::string& name, int* textureID, unsigned count)
{
    GLCall(glUniform1iv(glGetUniformLocation(m_rendererID, name.c_str()), count, textureID));
}

int Shader::getUniformLocation(const std::string& name)
{
    // Only find uniform location if it has not been cached.
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
    
    // Find uniform.
    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1) 
    { 
        LUMEN_LOG_ERROR("Shader error : Uniform '" + name + "' does not exist.\n", "OPENGL SHADER")
    }
    // Add the uniform to the cache.
    m_uniformLocationCache[name] = location;

    // Return the uniform location.
    return location;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "Program")
    {
        GLCall( glGetShaderiv(shader, GL_COMPILE_STATUS, &success) );
        if (!success)
        {
            GLCall( glGetShaderInfoLog(shader, 1024, NULL, infoLog) );
            LUMEN_LOG_ERROR("Shader compilation type : " + type + infoLog, "OPENGL SHADER")
        }
    }
    else
    {
        GLCall( glGetProgramiv(shader, GL_LINK_STATUS, &success) );
        if (!success)
        {
            GLCall( glGetProgramInfoLog(shader, 1024, NULL, infoLog) );
            LUMEN_LOG_ERROR("Shader compilation type : " + type + infoLog, "OPENGL SHADER")
        }
        else
        {
            LUMEN_LOG_DEBUG(type + " shader linked.", "OPENGL SHADER");
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------