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

	// set uniforms
	void SetUniform4f(const std::string& name, const Vector4& vec);
private:
	ShaderProgramSource OpenShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	int GetUniformLocation(const std::string& name); 
};
#endif // SHADER_H
