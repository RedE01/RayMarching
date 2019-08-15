#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string loadFileToString(std::string filePath) {
	std::ifstream fileStream(filePath);

	if(!fileStream.is_open()) {
		std::cout << "ERROR: File: " << filePath << " could not be opened" << std::endl;
		return "";
	}

	std::stringstream fileBuffer;
	fileBuffer << fileStream.rdbuf();
	fileStream.close();

	return fileBuffer.str();
}

unsigned int createShader(std::string& shaderSource, GLenum shaderType) {
	unsigned int shaderId = glCreateShader(shaderType);

	const char* src = shaderSource.c_str();
	int srcLen = shaderSource.size();
	glShaderSource(shaderId, 1, &src, &srcLen);

	glCompileShader(shaderId);

	int params;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &params);
	if(params != GL_TRUE) {
		int logLen = 0;
		char message[1024];
		glGetShaderInfoLog(shaderId, 1024, &logLen, message);

		std::cout << message << std::endl;
	}

	return shaderId;
}

void getProgramErrorMessage(unsigned int programId, GLenum pname) {
	int params;
	glGetProgramiv(programId, pname, &params);
	if(params != GL_TRUE) {
		int logLen = 0;
		char message[1024];
		glGetProgramInfoLog(programId, 1024, &logLen, message);

		std::cout << message << std::endl;
	}
}

unsigned int createShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource) {
	unsigned int vertexShaderId = createShader(vertexShaderSource, GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	unsigned int programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);
	getProgramErrorMessage(programId, GL_LINK_STATUS);

	glValidateProgram(programId);
	getProgramErrorMessage(programId, GL_VALIDATE_STATUS);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

Renderer::Renderer(){
	if(glewInit() != GLEW_OK) {
		std::cout << "glewInit error" << std::endl;
	}

	std::string vertexShaderSrc = loadFileToString("src/vertexShader.glsl");
	std::string fragmentShaderSrc = loadFileToString("src/fragmentShader.glsl");
	shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc); 
	glUseProgram(shaderProgram);

	float verticies[] = {
		-1, -1,
		 1, -1,
		 1,  1,
		 1,  1,
		-1,  1,
		-1, -1
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
}

void Renderer::renderFrame() {
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES,0, 6);
}

void Renderer::setUniform2f(std::string uniformName, float val1, float val2) {
	int uniformLocation = getUniformLocation(uniformName);
	glUniform2f(uniformLocation, val1, val2);
}

void Renderer::setUniform3fv(std::string uniformName, int count, float* val) {
	int uniformLocation = getUniformLocation(uniformName);
	glUniform3fv(uniformLocation, count, val);
}

void Renderer::setUniform1i(std::string uniformName, int val) {
	int uniformLocation = getUniformLocation(uniformName);
	glUniform1i(uniformLocation, val);
}

void Renderer::setUniform1f(std::string uniformName, float val) {
	int uniformLocation = getUniformLocation(uniformName);
	glUniform1f(uniformLocation, val);
}

int Renderer::getUniformLocation(const std::string& uniformName) {
	return glGetUniformLocation(shaderProgram, uniformName.c_str());
}