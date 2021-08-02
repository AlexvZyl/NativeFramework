
// This unit does all of the shader hadling, including compiling and extracting the desired shader
// from the files.

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "shaderHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Default constructor.
Shader::Shader() 
{
    return;
};

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
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program.
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessary.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// Use the shader.
void Shader::use()
{
    glUseProgram(ID);
}

// Utility uniform functions.
void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// Utility function for checking shader compilation/linking errors.
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "[OPENGL ERROR][SHADER COMPILATION] Type: " << type << "\n" << infoLog << "\n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "[OPENGL ERROR][SHADER LINKING] Type: " << type << "\n" << infoLog << "\n";
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------