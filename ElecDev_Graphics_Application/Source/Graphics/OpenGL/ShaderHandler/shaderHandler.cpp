
// This unit does all of the shader hadling, including compiling and extracting the desired shader
// from the files.

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "shaderHandler.h"
#include "ErrorHandler/errorHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Constructor generates the shaders.
Shader::Shader(std::string& filePath)
{

    // Shader mode handler. Used as an index for the shaderSource.
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType shaderType = ShaderType::NONE;

    // Read shader from file.
    std::ifstream fileStream(filePath);
    // Stream that contains the shader.
    std::stringstream shaderSource[2];

    // Run through all of the lines.
    std::string line;
    while (std::getline(fileStream, line))
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
    const char* vShaderCode = vShaderString.c_str();
    const char* fShaderCode = fShaderString.c_str();

    // Compile the shaders.
    unsigned int vertex, fragment;

    // Vertex shader.
    GLCall( vertex = glCreateShader(GL_VERTEX_SHADER) );
    GLCall( glShaderSource(vertex, 1, &vShaderCode, NULL) );
    GLCall( glCompileShader(vertex) );
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader.
    GLCall( fragment = glCreateShader(GL_FRAGMENT_SHADER) );
    GLCall( glShaderSource(fragment, 1, &fShaderCode, NULL) );
    GLCall( glCompileShader(fragment) );
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program.
    GLCall( ID = glCreateProgram() );
    GLCall( glAttachShader(ID, vertex) );
    GLCall( glAttachShader(ID, fragment) );
    GLCall( glLinkProgram(ID) );
    checkCompileErrors(ID, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessary.
    GLCall( glDeleteShader(vertex) );
    GLCall( glDeleteShader(fragment) );
}

// Use the shader.
void Shader::use()
{
    GLCall( glUseProgram(ID) );
}

// Utility uniform functions.
void Shader::setBool(const std::string& name, bool value) const
{
    GLCall( glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value) );
}
void Shader::setInt(const std::string& name, int value) const
{
    GLCall( glUniform1i(glGetUniformLocation(ID, name.c_str()), value) );
}
void Shader::setFloat(const std::string& name, float value) const
{
    GLCall( glUniform1f(glGetUniformLocation(ID, name.c_str()), value) );
}
void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    GLCall( glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value) ) );
}
// Utility function for checking shader compilation/linking errors.
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        GLCall( glGetShaderiv(shader, GL_COMPILE_STATUS, &success) );
        if (!success)
        {
            GLCall( glGetShaderInfoLog(shader, 1024, NULL, infoLog) );
            std::cout << "[OPENGL ERROR][SHADER COMPILATION] Type: " << type << "\n" << infoLog << "\n";
        }
    }
    else
    {
        GLCall( glGetProgramiv(shader, GL_LINK_STATUS, &success) );
        if (!success)
        {
            GLCall( glGetProgramInfoLog(shader, 1024, NULL, infoLog) );
            std::cout << "[OPENGL ERROR][SHADER LINKING] Type: " << type << "\n" << infoLog << "\n";
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------