
// This unit does all of the shader hadling, including compiling and extracting the desired shader
// from the files.

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "shaderHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Constructor generates the shaders.
Shader::Shader(std::string& filePath)
    : m_filePath(filePath), m_rendererID(0)
{

    // Shader mode handler. Used as an index for the shaderSource.
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType shaderType = ShaderType::NONE;

    // Parse the shader.
    //----------------------------------------------------------------------------

    // Read shader from file.
    std::ifstream fileStream(m_filePath);
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

    //----------------------------------------------------------------------------

    // Compile the shader program.
    compileShader(vShaderCode, fShaderCode); 

}

// Destructor
Shader::~Shader()
{
    std::cout << "[OPENGL][SHADER] Destructor called.\n\n";
    //GLCall(glDeleteProgram(m_rendererID));
}

// Compiles the shader program.
void Shader::compileShader(const char* vShaderCode, const char* fShaderCode)
{
    // Compile the shaders.
    unsigned int vertex, fragment;

    // Vertex shader.
    GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GLCall(glCompileShader(vertex));
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader.
    GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GLCall(glCompileShader(fragment));
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program.
    GLCall(m_rendererID = glCreateProgram());
    GLCall(glAttachShader(m_rendererID, vertex));
    GLCall(glAttachShader(m_rendererID, fragment));
    GLCall(glLinkProgram(m_rendererID));
    checkCompileErrors(m_rendererID, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessary.
    GLCall(glDeleteShader(vertex));
    GLCall(glDeleteShader(fragment));
}

// Use the shader.
void Shader::bind()
{
    GLCall( glUseProgram( m_rendererID ) );
}

// Use the shader.
void Shader::unbind()
{
    GLCall(glUseProgram(0));
}

// Utility uniform functions.
void Shader::setBool(const std::string& name, bool value)
{
    GLCall( glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), (int)value) );
}
void Shader::setInt(const std::string& name, int value)
{
    GLCall( glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), value) );
}
void Shader::setFloat(const std::string& name, float value)
{
    GLCall( glUniform1f(glGetUniformLocation(m_rendererID, name.c_str()), value) );
}
void Shader::setMat4(const std::string& name, glm::mat4 value)
{
    // Check if the uniform exists.
    GLCall ( glUniformMatrix4fv( getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value) ) );
}

// Get the uniform location.
int Shader::getUniformLocation(const std::string& name)
{
    // Only find uniform location if it has not been cached.
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
    
    // Find uniform.
    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1) { std::cout << "[SHADER ERROR] Uniform '" << name << "' does not exist.\n"; }
    // Add the uniform to the cache.
    m_uniformLocationCache[name] = location;

    // Return the uniform location.
    return location;
}

// Check shader compiling and linking errors.
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
            std::cout << "[OPENGL][ERROR][SHADER COMPILATION] Type: " << type << "\n" << infoLog << "\n";
        }
        else 
        {
            // Print success message.
            std::cout << "[OPENGL][SHADERS][" << type <<  "] Compiled succesfully.\n";
        }

    }
    else
    {
        GLCall( glGetProgramiv(shader, GL_LINK_STATUS, &success) );
        if (!success)
        {
            GLCall( glGetProgramInfoLog(shader, 1024, NULL, infoLog) );
            std::cout << "[OPENGL][ERROR][SHADER LINKING] Type: " << type << "\n" << infoLog << "\n";
        }
        else
        {
            // Print success message.
            std::cout << "[OPENGL][SHADERS][" << type << "] Linked succesfully.\n\n";
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------