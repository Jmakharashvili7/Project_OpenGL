#pragma once 
#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "structures.h"

class Shader
{
private:
	std::string m_filepath;
	unsigned int m_rendererID;
	// caching for uniforms
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value);
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value);
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value);
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value);
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, float x, float y);
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value);
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, float x, float y, float z);
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value);
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, float x, float y, float z, float w);
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat);
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat);
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat);
    // ------------------------------------------------------------------------
private:
	ShaderProgramSource ParceShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	int GetUniformLocation(const std::string& name); 
};
#endif // SHADER_H
