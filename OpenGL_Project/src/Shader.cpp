#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // if geometry shader path is present, also load a geometry shader
        if (geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure& e)
    {
        LOG("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // if geometry shader is given, compile geometry shader
    unsigned int geometry;

    if (geometryPath != nullptr)
    {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    // shader Program
    m_rendererID = glCreateProgram();
    glAttachShader(m_rendererID, vertex);
    glAttachShader(m_rendererID, fragment);

    if (geometryPath != nullptr)
        glAttachShader(m_rendererID, geometry);

    glLinkProgram(m_rendererID);
    checkCompileErrors(m_rendererID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (geometryPath != nullptr)
        glDeleteShader(geometry);

}

void Shader::Bind()
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            // check if the shader was compiled
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                // print the error
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                LOG("ERROR::SHADER_COMPILATION_ERROR of type: " + type + " " + infoLog);
            }
        }
        else
        {
            // check if the shader was linked
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                // print the error
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                LOG("ERROR::PROGRAM_LINKING_ERROR of type: " + type + " " + infoLog);
            }
        }
    }
}

void Shader::setBool(const std::string& name, bool value) const
{
    GLCall(glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), (int)value));
}

void Shader::setInt(const std::string& name, int value) const
{
    GLCall(glUniform1i(glGetUniformLocation(m_rendererID, name.c_str()), value));
}

void Shader::setFloat(const std::string& name, float value) const
{
    GLCall(glUniform1f(glGetUniformLocation(m_rendererID, name.c_str()), value));
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    GLCall(glUniform2fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, &value[0]));
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    GLCall(glUniform2f(glGetUniformLocation(m_rendererID, name.c_str()), x, y));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    GLCall(glUniform3fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, &value[0]));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    GLCall(glUniform3f(glGetUniformLocation(m_rendererID, name.c_str()), x, y, z));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    GLCall(glUniform4fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, &value[0]));
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
    GLCall(glUniform4f(glGetUniformLocation(m_rendererID, name.c_str()), x, y, z, w));
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    GLCall(glUniformMatrix2fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    GLCall(glUniformMatrix3fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    GLCall(glUniformMatrix4fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
