#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_rendererID(0)
{
	ShaderProgramSource source = ParceShader(filepath);
	m_rendererID = CreateShader(source.VertexSource, source.FragmentShader);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_rendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_rendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// create a program object to use the shaders
	GLCall(unsigned int program = glCreateProgram());

	// compile the vertex and fragment shader
	unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attach both shaders to the program
	GLCall(glAttachShader(program, vShader));
	GLCall(glAttachShader(program, fShader));

	// link the program and validate it 
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// Delete the shaders after they have been attached to the progam
	GLCall(glDeleteShader(vShader));
	GLCall(glDeleteShader(fShader));

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// Generate an m_rendererID for the shader
	GLCall(unsigned int m_rendererID = glCreateShader(type));

	// Create char array as OpenGL doesnt accept string
	const char* src = source.c_str();

	// Pass in the source code to the shader and compile the shader (will replace previous shader code)
	GLCall(glShaderSource(m_rendererID, 1, &src, nullptr));

	//compile the shader and bind it to an m_rendererID
	GLCall(glCompileShader(m_rendererID));

	// temp variable result to hold the state of compile status
	int result;
	GLCall(glGetShaderiv(m_rendererID, GL_COMPILE_STATUS, &result));

	// if the compilation failed 
	if (result == GL_FALSE)
	{
		// get the length of the log message
		int length;
		GLCall(glGetShaderiv(m_rendererID, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));

		// store the log message
		GLCall(glGetShaderInfoLog(m_rendererID, length, &length, message));

		// Let the user know which shader failed to compile
		std::cout << "Failed to compile"  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader" << std::endl;

		// print out the error 
		std::cout << message << std::endl;

		// delete the shader as it was not compiled 
		GLCall(glDeleteShader(m_rendererID));

		return 0;
	}

	return m_rendererID;
}

ShaderProgramSource Shader::ParceShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAMGENT = 1
	};

	// string for the current line we're reading 
	std::string line;

	// string stream to store the file data
	std::stringstream ss[2];

	// variable to hold the shader type
	ShaderType type = ShaderType::NONE;

	// loop through the file 
	while (getline(stream, line))
	{
		// Determine the shader type
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAMGENT;
		}
		// if there is no #shader we need to store the data 
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

int Shader::GetUniformLocation(const std::string& name)
{
	GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
	if (location == -1)
	{
		LOG("Waning: uniform " + name + " doesn't exist!");
	}
	return location;
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(m_rendererID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(m_rendererID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(m_rendererID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat)
{
	glUniformMatrix2fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(m_rendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


