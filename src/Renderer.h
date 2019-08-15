#pragma once
#include <GL/glew.h>
#include <string>

class Renderer {
public:
	int shaderProgram;

private:
	unsigned int vbo;

public:
	Renderer();

	void renderFrame();

	void setUniform2f(std::string uniformName, float val1, float val2);
	void setUniform3fv(std::string uniformName, int count, float* val);
	void setUniform1i(std::string uniformName, int val);
	void setUniform1f(std::string uniformName, float val);

private:
	int getUniformLocation(const std::string& uniformName);
};