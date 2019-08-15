#pragma once
#include <GLFW/glfw3.h>

class Window {
public:

private:
	GLFWwindow* window;
	int width, height;

public:
	Window();
	~Window();

	bool shouldClose();
	void setShouldClose(bool value);
	void swapBuffer();
	void pollEvents();

	int getWidth() { return width; }
	int getHeight() { return height; }
	inline const GLFWwindow* const getWindow() const { return window; }
};