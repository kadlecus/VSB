#include "Shader.h"
#include <GL/glew.h>
#include "stdio.h"
#include <fstream>
#include <iostream>

Shader::Shader(){}

Shader::Shader(const char* shaderString, GLenum type)
{
	this->id = glCreateShader(type);
	glShaderSource(this->id, 1, &shaderString, NULL);
	glCompileShader(this->id);

}

//pridano
void Shader::createShader(GLenum shaderType, const char* shaderCode)
{
    // Creates an empty shader
    this->id = glCreateShader(shaderType);

    // Sets the source code of the shader.
    glShaderSource(this->id, 1, &shaderCode, NULL);

    // Compiles the shader source code
    glCompileShader(this->id);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetShaderInfoLog(this->id, logLength, nullptr, &log[0]);

        std::cerr << "Shader compilation failed:\n" << log << std::endl;
    }
}

void Shader::createShaderFromFile(GLenum shaderType, const char* shaderFile)
{
	//Loading the contents of a file into a variable
	std::ifstream file(shaderFile);
	if (!file.is_open())
	{
		std::cout << "Unable to open file " << shaderFile << std::endl;
		exit(-1);
	}
	std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	createShader(shaderType, shaderCode.c_str());

}

void Shader::attachShader(GLuint idShaderProgram)
{
	//Attaches the shader to the shaderProgram
	glAttachShader(idShaderProgram, this->id);
}