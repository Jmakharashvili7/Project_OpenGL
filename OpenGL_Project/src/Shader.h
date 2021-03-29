#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <sstream> 

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	// Generate an ID for the shader
	unsigned int id = glCreateShader(type);

	// Create char array as OpenGL doesnt accept string
	const char* src = source.c_str();

	// Pass in the source code to the shader and compile the shader (will replace previous shader code)
	glShaderSource(id, 1, &src, nullptr);

	//compile the shader and bind it to an ID
	glCompileShader(id);

	// temp variable result to hold the state of compile status
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	// if the compilation failed 
	if (result == GL_FALSE)
	{
		// get the length of the log message
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		// store the log message
		glGetShaderInfoLog(id, length, &length, message);

		// Let the user know which shader failed to compile
		std::cout << "Failed to compile"  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader" << std::endl;

		// print out the error 
		std::cout << message << std::endl;

		// delete the shader as it was not compiled 
		glDeleteShader(id);

		return 0;
	}

	return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// create a program object to use the shaders
	unsigned int program = glCreateProgram();

	// compile the vertex and fragment shader
	unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attach both shaders to the program
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	// link the program and validate it 
	glLinkProgram(program);
	glValidateProgram(program);

	// Delete the shaders after they have been attached to the progam
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return program;
}

static ShaderProgramSource ParseShader(const std::string& filepath)
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

#endif // SHADER_H

